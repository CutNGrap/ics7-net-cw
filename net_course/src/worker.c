#include "worker.h"

#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/socket.h>

#include "connection.h"
#include "log.h"
#include "server.h"

void run_worker(worker_t *worker) {
    // Parent death
    prctl(PR_SET_PDEATHSIG, SIGHUP);
    int servsock = get_servsock();
    if (listen(servsock, 1024) == -1) {
        log_error("Cannot listen to address");
        return;
    }
    while (1) {
        wait_for_fd();
        if (is_fd_set(servsock)) {
            connection_t *connection = create_connection(servsock);
            set_connection_by_fd(connection->fd, connection);
            set_fd(connection->fd);
        }
        for (size_t i = 0; i < MAX_FD; i++) {
            if (is_fd_set(i)) {
                connection_t *connection = get_connection_by_fd(i);
                if (connection) {
                    handle_connection(connection);
                    clr_fd(connection->fd);
                    set_connection_by_fd(connection->fd, NULL);
                    close_connection(connection);
                }
            }
        }
    }
}
