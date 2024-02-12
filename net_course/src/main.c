#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "log.h"
#include "server.h"

#define SLEEP 100000L  // 100 ms

void int_handler(int sign) {
    stop_server();
    log_info("Shutting down");
    exit(EXIT_SUCCESS);
}

#define PUBLIC_PATH "./data/"

pthread_mutex_t logmtx = PTHREAD_MUTEX_INITIALIZER;
void log_lock(bool lock, void *udata) {
    if (lock) {
        pthread_mutex_lock(&logmtx);
    } else {
        pthread_mutex_unlock(&logmtx);
    }
}

int main(void) {
    log_add_fp(fopen("log.log", "a+"), LOG_TRACE);
    log_set_quiet(true);
    log_set_lock(log_lock, NULL);
    log_info("Server starting");
    chdir(PUBLIC_PATH);
    chroot(PUBLIC_PATH);

    signal(SIGINT, int_handler);

    init_server();
    while (1) {
        usleep(SLEEP);
    }
    return 0;
}
