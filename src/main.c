#include "../include/thread_pool.h"
#include "../include/arguments.h"
#include "../include/network.h"
#include "../include/serialization.h"
#include "../include/sequencer.h"
#include "../include/base64.h"
#include "../include/tls.h"
#include "../include/tcp.h"
#include <netdb.h>
#include <stdio.h>

int main(int argc, char **argv) {

    /* Parse all command-line arguments. */
    arguments_t arguments = parse_arguments(argc, argv);

    /* See if Transport Layer Security certificates and keys are specified. */
    SSL_CTX *ssl_context = NULL;
    if (arguments.tls_certificate && arguments.tls_certificate_key) {

        /* If they are specified, we should create an appropriate context. */
        if (arguments.workerMode) {

            /* Create a client context. */
            ssl_context = create_ssl_client_context(arguments.tls_certificate, arguments.tls_certificate_key);
            if (ssl_context == NULL) {

                fprintf(stderr, "Failed to create SSL context for client.\n");
                return EXIT_FAILURE;

            }

        } else {

            /* Create a server context. */
            ssl_context = create_ssl_server_context(arguments.tls_certificate, arguments.tls_certificate_key);
            if (ssl_context == NULL) {

                fprintf(stderr, "Failed to create SSL context for server.\n");
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
            
            /* If we've created a SSL context, we'll destroy that as well. */
            if (ssl_context) {

                SSL_CTX_free(ssl_context);

            }

            return EXIT_FAILURE;
    
        }

    }

    /* Create networking thread pool. */
    thread_pool_t *network_thread_pool = create_thread_pool(arguments.network_threads);
    if (!network_thread_pool) {

        fprintf(stderr, "Failed to create network thread pool.\n");

        /* If we've created a task thread pool but failed to create the network thread pool, we should tear down the task thread pool. */
        if (!task_thread_pool) {

            thread_pool_destroy(task_thread_pool);
        
        }

        /* If we've created a SSL context, we'll destroy that as well. */
        if (ssl_context) {

            SSL_CTX_free(ssl_context);

        }

        return EXIT_FAILURE;

    }

    /* Convert our human-readable address and port into a machine-readable format. */
    unsigned int sockaddr_length;
    struct sockaddr *sockaddr = create_sockaddr(arguments.host, arguments.port, &sockaddr_length);

    /* Check for errors. */
    if (!sockaddr) {

        fprintf(stderr, "Failed to create sockaddr structure.\n");
    
        /* Tear down any residual data structures. */
        if (task_thread_pool) {
   
            thread_pool_destroy(task_thread_pool);
    
        }
    
        if (network_thread_pool) {
    
            thread_pool_destroy(network_thread_pool);
    
        }
    
        if (ssl_context) {
    
            SSL_CTX_free(ssl_context);
    
        }
        
        return EXIT_FAILURE;
    
    }

    /* Branch if we're a controller or a worker. */
    if (!arguments.workerMode) {

        /* Create our task list. */
        task_list_t *task_list = NULL;

        /* Create a socket. */
        int sockfd = create_socket(sockaddr, sockaddr_length, arguments.timeout);

        /* Check for any errors. */
        if (sockfd == -1) {

            fprintf(stderr, "Failed to open a socket.\n");

            /* Tear down any residual data structures. */
            if (task_thread_pool) {

                thread_pool_destroy(task_thread_pool);

            }

            if (network_thread_pool) {

                thread_pool_destroy(network_thread_pool);

            }

            if (ssl_context) {

                SSL_CTX_free(ssl_context);

            }

            if (sockaddr) {

                free(sockaddr);

            }

            return EXIT_FAILURE;

        }

        /* If encryption is enabled, we should branch into a routine for an encrypted server. Otherwise, treat it like an ordinary plaintext connection. */
        if (ssl_context) {

            /* Populate a necessary structure that we'll rely on later. */
            task_list = (task_list_t *) malloc(sizeof(struct task_list));
            pthread_mutex_init(&task_list->pending_tasks_mutex, NULL)
            task_list->pending_tasks = NULL;
            tls_server(ssl_context, network_thread_pool, sockfd, task_list);

        } else {

            

        }

    } else {

        /* We are a worker. */
        printf("Worker mode.\n");

    }

    /* Destroy the thread pools and exit the program. */
    if (task_thread_pool) {

        thread_pool_destroy(task_thread_pool);

    }

    if (network_thread_pool) {

        thread_pool_destroy(network_thread_pool);

    }

    /* Free the socket address structure if it exists. */
    if (sockaddr) {

        free(sockaddr);

    }

    /* If we setup any contexts that we haven't destroyed already, we should also do that as well. */
    if (ssl_context) {

        SSL_CTX_free(ssl_context);

    }

    return EXIT_SUCCESS;

}
