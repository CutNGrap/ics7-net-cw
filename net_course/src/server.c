#include "server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <pthread.h>

#include "log.h"

static pthread_mutex_t conmtx = PTHREAD_MUTEX_INITIALIZER;

static connection_t *connections[MAX_FD] = {0};

connection_t *get_connection_by_fd(int fd) {
    pthread_mutex_lock(&conmtx);
    connection_t *con = connections[fd];
    pthread_mutex_unlock(&conmtx);
    return con;
}
void set_connection_by_fd(int fd, connection_t *connection) {
    pthread_mutex_lock(&conmtx);
    connections[fd] = connection;
    pthread_mutex_unlock(&conmtx);
}

static fd_set *rfds = NULL;
static worker_t *workers = NULL;
static size_t worker_count = 0;
static int max_fd = -1;
static int servsock = -1;

void sigchld_handler(int sig){}

int is_fd_set(int fd) {
    return FD_ISSET(fd, rfds);
}

int get_servsock() {
    return servsock;
}

void set_fd(int fd) {
    FD_SET(fd, rfds);
    if (fd > max_fd) {
        max_fd = fd;
    }
    return;
}

void clr_fd(int fd) {
    FD_CLR(fd, rfds);
    return;
}

int wait_for_fd() {
    if (pselect(max_fd + 1, rfds, NULL, NULL, NULL, NULL) == -1) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
int fork_worker(worker_t *worker);
int init_server() {
    close(STDIN_FILENO);
    close(STDERR_FILENO);
    signal(SIGCHLD, sigchld_handler);
    worker_count = get_nprocs();

    servsock = socket(AF_INET, SOCK_STREAM, 0);
    if (servsock == -1) {
        log_error("Cannot create socket");
        return EXIT_FAILURE;
    }
    if (setsockopt(servsock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) == -1) {
        close(servsock);
        log_error("Cannot set socket to reuse address");
        return EXIT_FAILURE;
    }
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(HOST);
    servaddr.sin_port = htons(PORT);
    if (bind(servsock, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        close(servsock);
        log_error("Cannot bind address");
        return EXIT_FAILURE;
    }

    max_fd = servsock;
    rfds = malloc(sizeof(fd_set));
    FD_ZERO(rfds);
    FD_SET(servsock, rfds);
    workers = malloc(sizeof(worker_t) * worker_count);
    for (size_t i = 0; i < worker_count; i++) {
        workers[i].servsock = servsock;
        if (fork_worker(workers + i) == EXIT_FAILURE) {
            free(workers);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int fork_worker(worker_t *worker) {
    pid_t pid = fork();
    if (pid == -1) {
        log_error("Cannot fork");
        return EXIT_FAILURE;
    } else if (pid > 0) {
        // parent
        log_info("Worker with pid %d forked", pid);
        worker->pid = pid;
    } else {
        // child
        run_worker(worker);
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}

void stop_server() {
    close(servsock);
    for (size_t i = 0; i < worker_count; i++)
    {
        kill(workers[i].pid, SIGKILL);
    }
}
