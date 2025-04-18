/*

    We aim to provide a facility to create and destroy network communication structures.

*/

#include "../../include/network.h"
#include <stdlib.h>

/* We aim to provide a facility to destroy network task requests. */
int destroy_net_task_request(net_task_request_t *net_task_request) {

    /* Ensure our input parameters are valid. */
    if (!net_task_request) {

        return -1;

    }

    /* Free the memory allocated for the routine file and arguments. */
    if (net_task_request->routine_file) {

        free(net_task_request->routine_file);

    }

    /* Free the memory allocated for the routine arguments. */
    if (net_task_request->routine_arguments) {

        free(net_task_request->routine_arguments);

    }

    /* Free the memory allocated for the net task request structure. */
    free(net_task_request);

    /* Return success. */
    return 0;

}