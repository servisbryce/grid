/*

    We aim to provide a facility for the user to be able to pass command-line arguments to the program.

*/

/* This is a structure containing the parameters set via command-line by the user. */
typedef struct arguments {

    bool workerMode;

} arguments_t;

/* This is a facility to convert command-line arguments into the arguments structure above. */
arguments_t parse_arguments(int argc, char **argv);