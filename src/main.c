#include "../include/thread_pool.h"
#include "../include/network.h"
#include "../include/serialization.h"
#include "../include/base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <dlfcn.h>

void *print(void *data) {

    printf("da\n");
    printf("how are ya doin?\n");
    printf("change412\n");
    printf("hi\n");
    int a = 0;
    a++;
    return data;

}

void main() {

    FILE *file_ptr = fopen("./test.so", "r");
    fseek(file_ptr, 0, SEEK_END);
    long fsize = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    char *memory = malloc(fsize + 1);
    fread(memory, fsize, 1, file_ptr);


    net_task_request_t *a = malloc(sizeof(net_task_request_t));
    a->routine_file = (void *) memory;
    a->routine_file_length = fsize;
    char *b = serialize_net_task_request(a);
    printf("%s\n", b);
    net_task_request_t *c = deserialize_net_task_request(b);

}