#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>

#include "worker.h"

#define HOST "127.0.0.1"
#define PORT 8080
#define MAX_FD 1024


#include "connection.h"

connection_t *get_connection_by_fd(int fd);
void set_connection_by_fd(int fd, connection_t *connection);
int init_server();
void stop_server();
void set_fd(int fd);
void clr_fd(int fd);
int wait_for_fd();
int is_fd_set(int fd);
int get_servsock();

#endif  // SERVER_H
