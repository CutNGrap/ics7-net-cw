#ifndef CONNECTION_H
#define CONNECTION_H

#define REQUEST_SIZE_LIMIT 4096
#define PATH_SIZE_LIMIT    256
#define METHOD_SIZE_LIMIT  16
#define STATUS_SIZE_LIMIT  64
#define HEADERS_SIZE_LIMIT 128
#define SEND_BUFFER_SIZE   1024

typedef struct request {
    char buffer[REQUEST_SIZE_LIMIT + 1];
    char path[PATH_SIZE_LIMIT + 1];
    char method[METHOD_SIZE_LIMIT + 1];
} request_t;

typedef struct response {
    char status[STATUS_SIZE_LIMIT + 1];
    char headers[HEADERS_SIZE_LIMIT + 1];
} response_t;

typedef struct connection {
    int fd;
    request_t request;
    response_t response;
    int is_served;
} connection_t;

connection_t *create_connection(int listen_sock);
void close_connection(connection_t *connection);
void handle_connection(connection_t *connection);

#endif // CONNECTION_H
