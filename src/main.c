#include "../include/thread_pool.h"
#include "../include/arguments.h"
#include "../include/network.h"
#include "../include/serialization.h"
#include "../include/sequencer.h"
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
                return EXIT_FAILURE;

            }

        } else {

            /* Create a client context. */
            ssl_context = create_ssl_server_context(arguments.tls_certificate, arguments.tls_certificate_key);
            if (ssl_context == NULL) {

                fprintf(stderr, "Failed to create SSL context for client.\n");
                return EXIT_FAILURE;

            }

        }

    }

    /* Create a task thread pool. */
    thread_pool_t *task_thread_pool = NULL;
    if (arguments.workerMode) {

        task_thread_pool = create_thread_pool(arguments.task_threads);
        if (!task_thread_pool) {

            fprintf(stderr, "Failed to create thread pool.\n");
            return EXIT_FAILURE;
    
        }

    }

    /* Create networking thread pool. */
    thread_pool_t *network_thread_pool = create_thread_pool(arguments.network_threads);
    if (!network_thread_pool) {

        fprintf(stderr, "Failed to create network thread pool.\n");
        return EXIT_FAILURE;

    }

    /* Branch if we're a controller or a worker. */
    if (!arguments.workerMode) {

        /* We are a controller. */
        printf("Controller mode.\n");

    } else {

        /* We are a worker. */
        printf("Worker mode.\n");

    }

    size_t length;
    char **a = disassemble_message("Hello, world!\n", 4, &length);
    char *b = reassemble_message(a, length);
    printf("Reassembled message: %s\n", b);

    /* Destroy the thread pools and exit the program. */
    if (task_thread_pool) {

        thread_pool_destroy(task_thread_pool);

    }

    if (network_thread_pool) {

        thread_pool_destroy(network_thread_pool);

    }

    /* If we setup any contexts that we haven't destroyed already, we should also do that as well. */
    if (ssl_context) {

        SSL_CTX_free(ssl_context);

    }

    return EXIT_SUCCESS;

}