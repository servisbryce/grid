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

    void *handle = dlopen("./test.so", RTLD_LAZY);
    void *(*routine)(void *routine_vargs_p);
    *(void **) (&routine) = dlsym(handle, "a");

    routine(NULL);

}