/* 

    We aim to provide a facility to disassemble and assemble messages so that they'll fit over a socket (standard maximum length of 64 kilobytes per message).

*/

#include "../../../include/sequencer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* We aim to provide a facility to splice strings. */
char *splice_string(char *string, size_t start, size_t length) {

    /* Ensure our string isn't null. */
    if (!string) {

        return NULL;

    }

    /* Allocate our string in the stack.*/
    char *spliced_string = (char *) malloc(length + 1);

    /* Ensure our allocation is successful. */
    if (!spliced_string) {

        return NULL;

    }

    strncpy(spliced_string, string + start, length);
    spliced_string[length + 1] = '\0';
    return spliced_string;

}

/* We aim to provide a function to disassemble any message into multiple different messages sized the same as or below the packet length. Not the most optimized solution, but it works for now. You could just programatically iterate over the string, but this is easier and more portable for the time being. */
char **disassemble_message(char *message, size_t packet_length, size_t *segmented_message_length) {

    /* Ensure our message isn't null. */
    if (!message) {

        return NULL;

    }

    /* Our message fits inside of the existing packet length already! */
    if (strlen(message) + 1 <= packet_length) {

        return NULL;

    }

    /* Set our static reused strings here. */
    char *packet_length_type = "Packet Length: ";
    char *newline = "\n";

    /* Count how many decimals are in our packet length to use in the tokenizer later. */
    size_t packet_length_decimals = 0;
    size_t packet_length_temporary = packet_length;
    while (packet_length_temporary > 0) {

        packet_length_temporary /= 10;
        packet_length_decimals++;

    }

    /* Tokenize our packet length. */
    char packet_length_tokenized[packet_length_decimals];
    sprintf(packet_length_tokenized, "%zu", packet_length);
    
    /* Allocate memory for our header. */
    char *segment_header = (char *) malloc(strlen(packet_length_type) + strlen(packet_length_tokenized) + strlen(newline) + 1);

    /* Assemble our header in the allocated memory on the stack. */
    strcpy(segment_header, packet_length_type);
    strcat(segment_header, packet_length_tokenized);
    strcat(segment_header, newline);

    /* Calculate how many segments we'll be making. */
    size_t segment_length = ((strlen(message) + 1) / packet_length) + ((strlen(message) + 1) % packet_length != 0 ? 1 : 0) + 1;
    *segmented_message_length = segment_length;

    /* Allocate our array of segments. */
    char **segmented_message = (char **) malloc(sizeof(char *) * segment_length);

    /* If the allocation fails, bail out. */
    if (!segmented_message) {

        free(segment_header);
        return NULL;

    }

    /* Create our segments. */
    for (size_t i = 0; i < segment_length; i++) {

        if (i == 0) {

            segmented_message[i] = segment_header;
            
        } else {

            size_t segment_data_length = i - 1;

            /* Splice our message down to size. */
            char *segment_content = splice_string(message, segment_data_length * packet_length, packet_length);

            /* If the memory allocation failed, abort. */
            if (!segment_content) {

                free(segment_header);
                for (size_t j = 0; j < segment_length; j++) {

                    if (segmented_message[j]) {
                        
                        free(segmented_message[j]);

                    }

                    free(segmented_message);

                }

                return NULL;

            }

            /* Add the allocated string to the array. */
            segmented_message[i] = segment_content;

        }

    }

    return segmented_message;

}

/* We aim to provide a facility to destroy a segmented message. */
int destroy_segmented_message(char **segmented_message, size_t segmented_message_length) {

    /* Ensure our segments aren't null. */
    if (!segmented_message) {

        return -1;

    }

    /* Free our segments. */
    for (size_t i = 0; i < segmented_message_length; i++) {

        if (segmented_message[i]) {

            free(segmented_message[i]);

        }

    }

    /* Free the array itself. */
    free(segmented_message);
    return 0;

}

/* We aim to provide a function to assemble multiple messages into a single message. */
char *reassemble_message(char **segmented_message, size_t segmented_message_length) {

    /* Ensure our segments aren't null. */
    if (!segmented_message) {

        return NULL;

    }

    /* See how large our message will be so we know how much we need to allocate from the heap later. We want to avoid using reallocate when possible as it's sometimes expensive. */
    size_t reassembled_message_length = 0;
    for (size_t i = 0; i < segmented_message_length; i++) {

        /* If our message segment exists, add up its size. */
        if (segmented_message[i]) {

            reassembled_message_length += strlen(segmented_message[i]);

        } else {

            /* If our message segment is incomplete, we shouldn't reassemble the message. Rather, we should tell the user something went wrong. */
            return NULL;

        }

    }

    /* Allocate our reassembled message. */
    char *reassembled_message = (char *) malloc(reassembled_message_length + 1);

    /* Ensure our allocation was successful. */
    if (!reassembled_message) {

        return NULL;

    }

    strcpy(reassembled_message, segmented_message[0]);
    for (size_t i = 1; i < segmented_message_length; i++) {

        /* If our message segment exists, add it to the reassembled message. */
        if (segmented_message[i]) {

            strcat(reassembled_message, segmented_message[i]);

        } else {

            /* If our message segment is incomplete, we shouldn't reassemble the message. Rather, we should tell the user something went wrong. */
            free(reassembled_message);
            return NULL;

        }

    }

    return reassembled_message;

}