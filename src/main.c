#include "../include/thread_pool.h"
#include "../include/arguments.h"
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

void main(int argc, char **argv) {

    arguments_t args = parse_arguments(argc, argv);
    printf("%ld\n", args.threads);
    printf("%s\n", args.tls_certificate);
    printf("%s\n", args.tls_certificate_key);

}