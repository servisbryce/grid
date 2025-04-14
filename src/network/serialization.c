/*

    We aim to provide a facility for the program to serialize and deserialize various network structures to allow them to be sent over a socket to a peer.

*/

#include "../../include/serialization.h"
#include "../../include/base64.h"
#include <stdlib.h>
#include <string.h>

/*

    We aim to provide a facility to serialize a network status structure to an encoded string.

*/

char *serialize_net_status(net_status_t *net_status) {

    /* Ensure our input parameters are valid. */
    if (!net_status) {

        return NULL;

    }

    size_t net_status_length = 0;
    char *net_status_type = "Type: net_status_t\n";
    char *net_status_key = "Status: ";
    char *net_status_value = NULL;
    if (net_status->status) {

        char *net_status_success = "Success.\n";
        net_status_value = encode(net_status_success, strlen(net_status_success) + 1, &net_status_length);

    } else {

        char *net_status_failure = "Failure.\n";
        net_status_value = encode(net_status_failure, strlen(net_status_failure) + 1, &net_status_length);

    }

    char *net_status_transmission = (char *) malloc(strlen(net_status_type) + strlen(net_status_key) + strlen(net_status_value) + 2);
    strcpy(net_status_transmission, net_status_type);
    strcat(net_status_transmission, net_status_key);
    strcat(net_status_transmission, net_status_value);
    strcat(net_status_transmission, "\n");
    return net_status_transmission;

}