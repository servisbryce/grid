/*

    We aim to provide a facility for the user to be able to pass command-line arguments to the program.

*/

#include <stddef.h>
#include <stdint.h>

/* This is a structure containing the parameters set via command-line by the user. */
typedef struct arguments {

    size_t task_threads;
    size_t network_threads;
    bool workerMode;
    char *tls_certificate;
    char *tls_certificate_key;
    char *host;
    uint16_t port;

} arguments_t;

/* This is a facility to convert command-line arguments into the arguments structure above. */
arguments_t parse_arguments(int argc, char **argv);