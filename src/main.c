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

    size_t a_buf = 0;
    char *msg = "test\n";
    char *a = encode(msg, strlen(msg) + 1, &a_buf);

    size_t b = 0;
    char *c = (char *) decode(a, &b);
    printf("%s", c);

}