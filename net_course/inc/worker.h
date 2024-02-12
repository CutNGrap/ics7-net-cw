#ifndef WORKER_H
#define WORKER_H

#include <sys/select.h>
#include <sys/types.h>

typedef struct worker {
    pid_t pid;
    int servsock;
} worker_t;

void run_worker(worker_t *worker);

#endif  // WORKER_H
