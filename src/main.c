#include "../include/thread_pool.h"
#include "../include/arguments.h"
#include "../include/network.h"
#include "../include/serialization.h"
#include "../include/base64.h"
#include "../include/tls.h"
#include <stdio.h>

int main(int argc, char **argv) {

    /* Parse all command-line arguments. */
    arguments_t arguments = parse_arguments(argc, argv);
    
    /* See if Transport Layer Security certificates and keys are specified. */
    SSL_CTX *ssl_context = NULL;
    if (arguments.tls_certificate && arguments.tls_certificate_key) {

        /* If they are specified, we should create an appropriate context. */
        if (arguments.workerMode) {

            /* Create a server context. */
            ssl_context = create_ssl_client_context(arguments.tls_certificate, arguments.tls_certificate_key);
            if (ssl_context == NULL) {

                fprintf(stderr, "Failed to create SSL context for server.\n");
                return -1;

            }

        } else {

            /* Create a client context. */
            ssl_context = create_ssl_server_context(arguments.tls_certificate, arguments.tls_certificate_key);
            if (ssl_context == NULL) {

                fprintf(stderr, "Failed to create SSL context for client.\n");
                return -1;

            }

        }

    }

}