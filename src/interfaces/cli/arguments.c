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

    /* Parse arguments passed by the user. */
    int option;
    while ((option = getopt(argc, argv, "wt:c:k:")) != -1) {

        switch (option) {

            case 'w':
                arguments.workerMode = true;
                break;

            case 't':
                arguments.threads = (size_t) strtoumax(optarg, NULL, 10);
                if (arguments.threads == 0) {

                    fprintf(stderr, "You must specify at least one thread: %s\n", optarg);
                    exit(EXIT_FAILURE);

                }

                break;

            case 'c':
                arguments.tls_certificate = strdup(optarg);
                break;

            case 'k':
                arguments.tls_certificate_key = strdup(optarg);
                break;

            default:
                fprintf(stderr, "Usage: %s [-w]\n", argv[0]);
                exit(EXIT_FAILURE);

        }

    }

    if ((arguments.tls_certificate && !arguments.tls_certificate_key) || (!arguments.tls_certificate && arguments.tls_certificate_key)) {

        fprintf(stderr, "You are either missing a certificate path or a certificate key path. You must have a certificate and a certificate key.\n");
        exit(EXIT_FAILURE);

    }

    return arguments;

}