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

/* We aim to provide a function to disassemble any message into multiple different messages sized the same as or below the packet length. */
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
    char **segments = (char **) malloc(sizeof(char *) * segment_length);

    /* If the allocation fails, bail out. */
    if (!segments) {

        free(segment_header);
        return NULL;

    }

    /* Create our segments. */
    for (size_t i = 0; i < segment_length; i++) {

        if (i == 0) {

            segments[i] = segment_header;
            
        } else {

            size_t segment_data_length = i - 1;

            /* Splice our message down to size. */
            char *segment_content = splice_string(message, segment_data_length * packet_length, packet_length);

            /* If the memory allocation failed, abort. */
            if (!segment_content) {

                free(segment_header);
                for (size_t j = 0; j < segment_length; j++) {

                    if (segments[j]) {
                        
                        free(segments[j]);

                    }

                    free(segments);

                }

                return NULL;

            }

            /* Add the allocated string to the array. */
            segments[i] = segment_content;

        }

    }

    return segments;

}

/* We aim to provide a function to assemble multiple messages into a single message. */
char *reassemble_message(char **messages) {



}