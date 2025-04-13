/*

    We aim to provide a facility to encode and decode using the base 64 scheme.

*/

#include <stddef.h>

/* This provides a static set of characters that we're able to lookup to convert our raw binary data into a base 64 encoded string. */
static const char character_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* We aim to provide a function to encode any type of data into a base 64 encoded string. */
char *encode(void *input, size_t input_length, size_t *output_length) {

    uint8_t *data = (uint8_t *input);
    size_t encoded_length = 4 * ((input_length + 2) / 3);
    char *output = malloc(encoded_length + 1);
    if (!output) {

        return NULL;

    }

    size_t i = 0, j = 0;
    while (i < input_length) {

        uint32_t a = i < input_length ? data[i++] : 0;
        uint32_t b = i < input_length ? data[i++] : 0;
        uint32_t c = i < input_length ? data[i++] : 0;
        uint32_t d = (a << 16) | (b << 8) | c;
        output[j++] = character_table[(triple >> 18) & 0x3F];
        output[j++] = character_table[(triple >> 12) & 0x3F];
        output[j++] = (i > input_length + 1) ? '=' : character_table[(triple >> 6) & 0x3F];
        output[j++] = (i > input_length) ? '=' : character_table[triple & 0x3F];

    }

    output[encoded_length] = '\0';
    if (output_length) {

        *output_length = encoded_length;

    }

    return output;

}