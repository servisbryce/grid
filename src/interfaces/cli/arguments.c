/*

    We aim to provide a facility for the user to be able to pass command line arguments to the program.

*/

#include "../../../include/arguments.h"
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

arguments_t parse_arguments(int argc, char **argv) {

    /* Create arguments structure in the stack. Make sure that the structure is clean before we use it by zeroing it out.  */
    arguments_t arguments;
    memset(&arguments, 0, sizeof(arguments));
    /* Assert default parameters */
    arguments.task_threads = 2;
    arguments.network_threads = 2;
    arguments.workerMode = false;

    /* Ensure that we don't end up with any possible unintentional dangling pointers. */
    arguments.tls_certificate = NULL;
    arguments.tls_certificate_key = NULL;
    arguments.host = NULL;

    /* Parse arguments passed by the user. */
    int option;
    while ((option = getopt(argc, argv, "wt:c:k:n:h:p:d:")) != -1) {

        switch (option) {

            case 'w':
                arguments.workerMode = true;
                break;

            case 't':
                arguments.task_threads = (size_t) strtol(optarg, NULL, 10);
                if (arguments.task_threads == 0) {

                    fprintf(stderr, "You must specify at least one task thread: %s\n", optarg);
                    exit(EXIT_FAILURE);

                }

                break;

            case 'n':
                arguments.network_threads = (size_t) strtol(optarg, NULL, 10);
                if (arguments.network_threads <= 1) {

                    fprintf(stderr, "You must specify at least more than one network thread: %s\n", optarg);
                    exit(EXIT_FAILURE);

                }

                break;

            case 'c':
                arguments.tls_certificate = strdup(optarg);
                break;

            case 'k':
                arguments.tls_certificate_key = strdup(optarg);
                break;

            case 'h':
                arguments.host = strdup(optarg);
                break;

            case 'p':
                arguments.port = (uint16_t) strtol(optarg, NULL, 10);
                break;

            case 'd':
                arguments.timeout = (unsigned int) strtol(optarg, NULL, 10);
                break;

            default:
                printf("grid [options]\n");
                printf("Options:\n");
                printf("  -w\t\tRun in worker mode.\n");
                printf("  -t <threads>\tSet the number of task threads to use.\n");
                printf("  -n <threads>\tSet the number of network threads to use.\n");
                printf("  -c <path>\tSet the path to the TLS certificate.\n");
                printf("  -k <path>\tSet the path to the TLS certificate key.\n");
                printf("  -h <host>\tSet the host to bind or connect to.\n");
                printf("  -p <port>\tSet the port to bind or connect to.\n");
                printf("  -d <timeout>\tSet the timeout for the socket in seconds.\n");
                exit(EXIT_FAILURE);

        }

    }

    /* Ensure that the user has selected both a certificate and a key, not one or the other. */
    if ((arguments.tls_certificate && !arguments.tls_certificate_key) || (!arguments.tls_certificate && arguments.tls_certificate_key)) {

        fprintf(stderr, "You are either missing a certificate path or a certificate key path. You must have a certificate and a certificate key.\n");
        exit(EXIT_FAILURE);

    }

    /* If we're a worker and there's no controller address set, then throw an error. */
    if (arguments.workerMode && !arguments.host) {

        fprintf(stderr, "You must specify a host when using worker mode.\n");
        exit(EXIT_FAILURE);

    }

    /* If we're a controller, then just accept all incoming connections on port 15411 if there aren't any options set. */
    if (!arguments.workerMode && !arguments.host) {

        arguments.host = "0.0.0.0";

    }

    /* If the user hasn't specified a port, then we should set it to the protocol default 15411. */
    if (arguments.port <= 1) {
        
        arguments.port = 15411;

    }

    return arguments;

}