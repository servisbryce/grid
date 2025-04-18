/*

    We aim to provide a facility for the user to be able to pass command line arguments to the program.

*/

#include "../../../include/arguments.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

arguments_t parse_arguments(int argc, char **argv) {

    /* Create arguments structure in the stack. Make sure that the structure is clean before we use it by zeroing it out.  */
    arguments_t arguments;
    memset(&arguments, 0, sizeof(arguments));

    int option;
    while ((option = getopt(argc, argv, "w")) != -1) {

        switch (option) {

            case 'w':
                arguments.workerMode = true;
                break;

            default:
                fprintf(stderr, "Usage: %s [-w]\n", argv[0]);
                exit(EXIT_FAILURE);

        }

    }

}