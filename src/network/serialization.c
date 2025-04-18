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

/* We aim to provide a facility to deserialize a serialized network status string. */
net_status_t *deserialize_net_status(char *serialized_net_status) {

    /* Ensure our input parameters are valid. */
    if (!serialized_net_status) {

        return NULL;

    }

    /* Create a value where we'll store our object that we're creating. */
    net_status_t *net_status = NULL;

    /* You need to duplicate the string because the string tokenizer modifies the string whenever we perform operations. */
    char *net_status_serialized = strdup(serialized_net_status);

    /* We also need to create a context so that we're able to remember where we are in the string for a given string tokenizer call. */
    char *line_context = NULL;

    /* We're going to loop through every line in the string. */
    char *current_line = strtok_r(net_status_serialized, "\n", &line_context);
    while (current_line) {

        /* We are going to then seperate each line into their type and value components.*/
        char *type = strtok(current_line, ": ");
        char *value = strtok(NULL, ": ");

        /* If there's no valid string for either the type or value, then we'll just iterate to the next line. */
        if (!type || !value) {

            current_line = strtok_r(NULL, "\n", &line_context);
            continue;

        }

        /* These two statements are responsible for determining if we're creating a network status structure. If we are, then create it. If not, then just don't do anything. */
        if (strcmp(type, "Type") == 0) {

            if (strcmp(value, "net_status_t") == 0) {

                net_status = (net_status_t *) malloc(sizeof(net_status_t));
                current_line = strtok_r(NULL, "\n", &line_context);
                continue;

            }

        }

        /* We need to ensure that the object has been defined first, if not then we may encounter memory issues. */
        if (!net_status) {

            current_line = strtok_r(NULL, "\n", &line_context);
            continue;

        }

        /* These statements are responsible for determining if this line's context is a status. If it is a status, then decode the corresponding value and file out the network status structure. */
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

    /* Return the completed structure. */
    return net_status;

}

/* We aim to provide a facility to serialize network task requests.*/
char *serialize_net_task_request(net_task_request_t *net_task_request) {

    /* Validate our parameters. */
    if (!net_task_request) {

        return NULL;

    }

    /* Hold our constant strings here. */
    char *newline = "\n";

    /* Assemble the headers. */
    char *net_task_request_type = "Type: net_task_request_t\n";

    /* Assemble the content keys and encoded values. */
    char *net_task_request_identifier_key = "Identifier: ";
    size_t net_task_request_identifier_value_length = 0;
    char *net_task_request_identifier_value = encode(&net_task_request->identifier, sizeof(net_task_request->identifier), &net_task_request_identifier_value_length);

    /* We're going to encode the routine file. */
    size_t encoded_routine_file_length = 0;
    char *net_task_request_routine_key = "Routine File: ";
    char *net_task_request_routine_value = encode(net_task_request->routine_file, net_task_request->routine_file_length, &encoded_routine_file_length);

    /* We're also going to encode the routine arguments as well. */
    size_t encoded_routine_arguments_length = 0;
    char *net_task_request_routine_arguments_key = "Routine Arguments: ";
    char *net_task_request_routine_arguments_value = encode(net_task_request->routine_arguments, net_task_request->routine_arguments_length, &encoded_routine_arguments_length);

    /* Assemble the serialized network task request message. */
    char *serialized_net_task_request = (char *) malloc(strlen(net_task_request_type) + strlen(net_task_request_identifier_key) + strlen(net_task_request_identifier_value) + strlen(newline) + strlen(net_task_request_routine_key) + strlen(net_task_request_routine_value) + strlen(newline) + strlen(net_task_request_routine_arguments_key) + strlen(net_task_request_routine_arguments_value) + strlen(newline) + 1);
    strcpy(serialized_net_task_request, net_task_request_type);
    strcat(serialized_net_task_request, net_task_request_identifier_key);
    strcat(serialized_net_task_request, net_task_request_identifier_value);
    strcat(serialized_net_task_request, newline);
    strcat(serialized_net_task_request, net_task_request_routine_key);
    strcat(serialized_net_task_request, net_task_request_routine_value);
    strcat(serialized_net_task_request, newline);
    strcat(serialized_net_task_request, net_task_request_routine_arguments_key);
    strcat(serialized_net_task_request, net_task_request_routine_arguments_value);
    strcat(serialized_net_task_request, newline);

    /* Return the serialized network task request message.*/
    return serialized_net_task_request;

}

/* We aim to provide a facility to deserialize a serialized network task request message. */
net_task_request_t *deserialize_net_task_request(char *serialized_net_task_request) {

    /* Validate that our inputs aren't null to prevent memory issues. */
    if (!serialized_net_task_request) {

        return NULL;

    }

    /* Duplicate our original string because we'll be modifying this string during the translation process. */
    char *net_task_request_serialized = strdup(serialized_net_task_request);

    /* We'll also need to setup a line context because we'll be jumping from multiple different tokenization contexts within all branches of this function. */
    char *line_context = NULL;

    /* Create our object. Parse the message, line by line. */
    net_task_request_t *net_task_request = NULL;
    char *current_line = strtok_r(net_task_request_serialized, "\n", &line_context);
    while (current_line) {

        /* Parse the type and key of the line. */
        char *type = strtok(current_line, ": ");
        char *key = strtok(NULL, ": ");

        /* If one or the other don't exist, then halt immediately as this line is corrupted. */
        if (!type || !key) {

            current_line = strtok_r(NULL, "\n", &line_context);
            continue;

        }

        /* Check if the structure we want to create is defined. If so, then create it. */
        if (strcmp(type, "Type") == 0) {

            if (strcmp(key, "net_task_request_t") == 0) {

                net_task_request = (net_task_request_t *) malloc(sizeof(net_task_request_t));
                current_line = strtok_r(NULL, "\n", &line_context);
                continue;

            }

        }

        /* We need to ensure that the object has been defined first, if not then we may encounter memory issues. */
        if (!net_task_request) {

            current_line = strtok_r(NULL, "\n", &line_context);
            continue;

        }

        /* If our identifier is set, then we'll set the identifier in our object accordingly. */
        if (strcmp(type, "Identifier") == 0) {

            size_t decoded_value_length = 0;
            size_t *decoded_value = decode(key, &decoded_value_length);
            net_task_request->identifier = *decoded_value;
            current_line = strtok_r(NULL, "\n", &line_context);
            continue;

        }

        /* If our routine file is set, then we'll load it into memory for processing later. */
        if (strcmp(type, "Routine File") == 0) {

            size_t decoded_value_length = 0;
            char *decoded_value = decode(key, &decoded_value_length);
            net_task_request->routine_file_length = decoded_value_length;
            net_task_request->routine_file = (void *) decoded_value;
            current_line = strtok_r(NULL, "\n", &line_context);
            continue;

        }

        /* If our routine arguments are set, then we'll load them into memory as well for processing later. */
        if (strcmp(type, "Routine Arguments") == 0) {

            size_t decoded_value_length = 0;
            char *decoded_value = decode(key, &decoded_value_length);
            net_task_request->routine_file_length = decoded_value_length;
            net_task_request->routine_file = (void *) decoded_value;
            current_line = strtok_r(NULL, "\n", &line_context);
            continue;

        }

    }

    /* Return our newly created structure. */
    return net_task_request;

}