/*

    We aim to provide a facility to encode and decode using the base 64 scheme.

*/

/* We aim to provide a function to encode any type of data into a base 64 encoded string. */
char *encode(void *input, size_t input_length, size_t *output_length);

/* We aim to provide a function to decode a base 64 encoded string into any data type. */
void *decode(char *input, size_t *output_length_p);