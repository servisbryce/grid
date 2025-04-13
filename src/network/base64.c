/*

    We aim to provide a facility to encode and decode using the base 64 scheme.

*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* This provides a static set of characters that we're able to lookup to convert our raw binary data into a base 64 encoded string. */
static const char character_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* This table provides an easy way to just lookup what each letter in a base 64 string corresponds to. */
static const uint8_t inverse_character_table[256] = {

    ['A']=0,  ['B']=1,  ['C']=2,  ['D']=3,  ['E']=4,  ['F']=5,  ['G']=6,  ['H']=7,
    ['I']=8,  ['J']=9,  ['K']=10, ['L']=11, ['M']=12, ['N']=13, ['O']=14, ['P']=15,
    ['Q']=16, ['R']=17, ['S']=18, ['T']=19, ['U']=20, ['V']=21, ['W']=22, ['X']=23,
    ['Y']=24, ['Z']=25, ['a']=26, ['b']=27, ['c']=28, ['d']=29, ['e']=30, ['f']=31,
    ['g']=32, ['h']=33, ['i']=34, ['j']=35, ['k']=36, ['l']=37, ['m']=38, ['n']=39,
    ['o']=40, ['p']=41, ['q']=42, ['r']=43, ['s']=44, ['t']=45, ['u']=46, ['v']=47, 
    ['w']=48, ['x']=49, ['y']=50, ['z']=51, ['0']=52, ['1']=53, ['2']=54, ['3']=55, 
    ['4']=56, ['5']=57, ['6']=58, ['7']=59, ['8']=60, ['9']=61, ['+']=62, ['/']=63

};

/* We aim to provide a function to encode any type of data into a base 64 encoded string. */
char *encode(void *input, size_t input_length, size_t *output_length) {

    uint8_t *data = (uint8_t *) input;
    size_t encoded_length = 4 * ((input_length + 2) / 3);
    char *output = (char *) malloc(encoded_length + 1);
    if (!output) {

        return NULL;

    }

    size_t i = 0, j = 0;
    while (i < input_length) {

        uint32_t a = i < input_length ? data[i++] : 0;
        uint32_t b = i < input_length ? data[i++] : 0;
        uint32_t c = i < input_length ? data[i++] : 0;
        uint32_t d = (a << 16) | (b << 8) | c;
        output[j++] = character_table[(d >> 18) & 0x3F];
        output[j++] = character_table[(d >> 12) & 0x3F];
        output[j++] = (i > input_length + 1) ? '=' : character_table[(d >> 6) & 0x3F];
        output[j++] = (i > input_length) ? '=' : character_table[d & 0x3F];

    }

    output[encoded_length] = '\0';
    if (output_length) {

        *output_length = encoded_length;

    }

    return output;

}

void *decode(char *input, size_t *output_length_p) {

    size_t input_length = strlen(input);
    if (input_length % 4 != 0) {

        return NULL;

    }

    size_t offset = 0;
    if (input_length >= 2 && input[input_length - 1] == '=') {
    
        offset++;
    
    }

    if (input_length >= 2 && input[input_length - 2] == '=') {

        offset++;

    }

    size_t output_length = (input_length / 4) * 3 - offset;
    uint8_t *output = (uint8_t *) malloc(output_length);
    if (!output) {

        return NULL;

    }

    size_t i = 0, j = 0;
    while (i < input_length) {

        uint32_t a = input[i] == '=' ? 0 & i++ : inverse_character_table[(int) input[i++]];
        uint32_t b = input[i] == '=' ? 0 & i++ : inverse_character_table[(int) input[i++]];
        uint32_t c = input[i] == '=' ? 0 & i++ : inverse_character_table[(int) input[i++]];
        uint32_t d = input[i] == '=' ? 0 & i++ : inverse_character_table[(int) input[i++]];
        uint32_t e = (a << 18) | (b << 12) | (c << 6) | d;
        if (j < output_length) {

            output[j++] = (e >> 16) & 0xFF;

        }

        if (j < output_length) {

            output[j++] = (e >> 8) & 0xFF;

        }

        if (j < output_length) {

            output[j++] = e & 0xFF;

        }

    }

    if (output_length) {

        *output_length_p = output_length;

    }

    return output;

}