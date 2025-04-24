/* 

    We aim to provide a facility to disassemble and assemble messages so that they'll fit over a socket (standard maximum length of 64 kilobytes per message).

*/

#include "../../../include/sequencer.h"
#include <string.h>
#include <stdio.h>

/* We aim to provide a function to disassemble any message into multiple different messages sized the same as or below the packet length. */
char **disassemble_message(char *message, size_t packet_length) {

    /* Ensure our message isn't null. */
    if (!message) {

        return NULL;

    }

    /* Set our static reused strings here. */
    char *segment_length_type = "Segment Length: ";
    char *segment_type = "Segment: ";
    char *newline = "\n";

    /* Calculate various lengths. */
    size_t message_length = strlen(message) + 1;
    size_t segment_length = (message_length / packet_length) + (message_length % packet_length != 0 ? 1 : 0);
    printf("Message Length: %zu\n", message_length);

}

/* We aim to provide a function to assemble multiple messages into a single message. */
char *reassemble_message(char **messages) {



}