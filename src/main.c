#include "../include/thread_pool.h"
#include "../include/network.h"
#include "../include/serialization.h"
#include "../include/base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *print(void *data) {

    thread_task_t *d = (thread_task_t *) data;
    printf("%s\n", (char *) d->routine_vargs_p);
    return data;

}

void main() {

    net_status_t *a = malloc(sizeof(net_status_t));
    a->status = false;
    char *b = serialize_net_status(a);
    printf("%s\n", b);
    net_status_t *c = deserialize_net_status(b);
    printf("%d\n", c->status);

}