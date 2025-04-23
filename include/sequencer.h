/* 

    We aim to provide a facility to disassemble and assemble messages so that they'll fit over a socket (standard maximum length of 64 kilobytes per message).

*/

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <stddef.h>

/* We aim to provide a function to disassemble any message into multiple different messages sized the same as or below the packet length. */
char **disassemble_message(char *message, size_t packet_length);

/* We aim to provide a function to assemble multiple messages into a single message. */
char *reassemble_message(char **messages);

#endif