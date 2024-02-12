#include "connection.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include "log.h"
#include "mime.h"

connection_t *create_connection(int listen_sock) {
    connection_t *connection = malloc(sizeof(connection_t));

    connection->fd = accept(listen_sock, NULL, NULL);
    connection->is_served = 0;

    return connection;
}

void close_connection(connection_t *connection) {
    close(connection->fd);
    free(connection);
    connection = NULL;
    return;
}

void write_file(FILE *file, int fd) {
    char data[SEND_BUFFER_SIZE];

    size_t n = 0;
    while ((n = fread(data, sizeof(char), SEND_BUFFER_SIZE, file)) > 0) {
        write(fd, data, n);
    }
}

#define RESPONSE_TEMPLATE     "HTTP/1.1 %d %s\r\n"
#define CONTENT_TYPE_TEMPLATE "Content-Type: %s\r\nContent-Length: %zu\r\n\r\n"

void handle_connection(connection_t *connection) {
    connection->is_served = 1;
    int fd = connection->fd;
    memset(connection->request.buffer, 0, REQUEST_SIZE_LIMIT + 1);
    read(fd, connection->request.buffer, REQUEST_SIZE_LIMIT - 1);

    sscanf(
      connection->request.buffer, "%s %s", connection->request.method, connection->request.path);

    if (strcmp(connection->request.method, "GET") == 0) {
        log_info("GET request");
        struct stat sb;
        char actual_path[PATH_SIZE_LIMIT + 3];
        if (strcmp(connection->request.path, "/") == 0) {
            sprintf(actual_path, "./index.html");
        } else {
            sprintf(actual_path, ".%s", connection->request.path);
        }

        int rc;
        if ((rc = stat(actual_path, &sb)) == 0) {
            const char *mime_type = detect_mime_type(actual_path);
            sprintf(connection->response.status, RESPONSE_TEMPLATE, 200, "OK");
            sprintf(connection->response.headers, CONTENT_TYPE_TEMPLATE, mime_type, sb.st_size);
        } else {
            sprintf(connection->response.status, RESPONSE_TEMPLATE, 404, "Not Found");
            write(fd, connection->response.status, strlen(connection->response.status));
        }
        if (rc == 0) {
            FILE *file = fopen(actual_path, "rb");
            if (file == NULL && errno == EACCES) {
                sprintf(connection->response.status, RESPONSE_TEMPLATE, 403, "Forbidden");
            }
            write(fd, connection->response.status, strlen(connection->response.status));
            write(fd, connection->response.headers, strlen(connection->response.headers));
            write_file(file, fd);
            fclose(file);
        }

    } else if (strcmp(connection->request.method, "HEAD") == 0) {
        log_info("HEAD request");
        struct stat sb;
        char actual_path[PATH_SIZE_LIMIT + 3];
        sprintf(actual_path, ".%s", connection->request.path);
        if (stat(actual_path, &sb) == 0) {
            const char *mime_type = detect_mime_type(actual_path);
            sprintf(connection->response.status, RESPONSE_TEMPLATE, 200, "OK");
            sprintf(connection->response.headers, CONTENT_TYPE_TEMPLATE, mime_type, sb.st_size);
        } else {
            sprintf(connection->response.status, RESPONSE_TEMPLATE, 404, "Not Found");
        }

        write(fd, connection->response.status, strlen(connection->response.status));
        write(fd, connection->response.headers, strlen(connection->response.headers));
    } else {
        log_info("Unknown request");
        sprintf(connection->response.status, RESPONSE_TEMPLATE, 405, "Method Not Allowed");
        write(fd, connection->response.status, strlen(connection->response.status));
    }
    // close_connection(connection);
}
