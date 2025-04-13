/*

    We aim to provide a facility for the program to serialize and deserialize various network structures to allow them to be sent over a socket to a peer.

*/

#include "../../include/serialization.h"
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

    /* We need this header to demark what structure we're trying to transmit. */
    char *structure_header = "net_status_t\n";

    /* Determine what our data payload will be. */
    char *structure_data = NULL;
    if (net_status->status) {

        structure_data = "status: success\n";

    } else {

        structure_data = "status: failure\n";

    }

    /* Load the data payload into the heap and ship it off to the program. */
    char *serialized_net_status = (char *) malloc(strlen(structure_header) + strlen(structure_data) + 1);
    strcpy(serialized_net_status, structure_header);
    strcat(serialized_net_status, structure_data);
    return serialized_net_status;

}