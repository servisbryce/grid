/*

    We aim to provide a facility for the program to serialize and deserialize various network structures to allow them to be sent over a socket to a peer.

*/

#include "../../include/serialization.h"
#include "../../include/base64.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* We aim to provide a facility to serialize a network status structure to an encoded string. */
char *serialize_net_status(net_status_t *net_status) {

    /* Ensure our input parameters are valid. */
    if (!net_status) {

        return NULL;

    }

    /* Assemble the headers. */
    size_t net_status_length = 0;
    char *net_status_type = "Type: net_status_t\n";

    /* Assemble the content key and encoded value. */
    char *net_status_key = "Status: ";
    char *net_status_value = NULL;
    if (net_status->status) {

        char *net_status_success = "Success\n";
        net_status_value = encode(net_status_success, strlen(net_status_success) + 1, &net_status_length);

    } else {

        char *net_status_failure = "Failure\n";
        net_status_value = encode(net_status_failure, strlen(net_status_failure) + 1, &net_status_length);

    }

    /* Assemble the headers and content into one string. */
    char *serialized_net_status = (char *) malloc(strlen(net_status_type) + strlen(net_status_key) + strlen(net_status_value) + 2);
    strcpy(serialized_net_status, net_status_type);
    strcat(serialized_net_status, net_status_key);
    strcat(serialized_net_status, net_status_value);
    strcat(serialized_net_status, "\n");

    /* Return the value. */
    return serialized_net_status;

}

net_status_t *deserialize_net_status(char *serialized_net_status) {

    if (!serialized_net_status) {

        return NULL;

    }

    net_status_t *net_status = NULL;
    char *net_status_serialized = strdup(serialized_net_status);
    char *line_context = NULL;
    char *current_line = strtok_r(net_status_serialized, "\n", &line_context);
    while (current_line != NULL) {

        char *type = strtok(current_line, ": ");
        char *value = strtok(NULL, ": ");
        if (strcmp(type, "Type") == 0) {

            if (strcmp(value, "net_status_t") == 0) {

                net_status = (net_status_t *) malloc(sizeof(net_status_t));
                current_line = strtok_r(NULL, "\n", &line_context);
                continue;

            }

        }

        if (strcmp(type, "Status") == 0) {

            size_t value_length = 0;
            char *value_decoded = decode(value, &value_length);
            if (strcmp(value_decoded, "Success\n") == 0) {

                net_status->status = true;
                current_line = strtok_r(NULL, "\n", &line_context);
                continue;

            } else if (strcmp(value_decoded, "Failure\n") == 0) {

                net_status->status = false;
                current_line = strtok_r(NULL, "\n", &line_context);
                continue;

            } else {

                current_line = strtok_r(NULL, "\n", &line_context);
                continue;

            }

        }

    }

    return net_status;

}