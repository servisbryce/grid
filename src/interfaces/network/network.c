/*

    We aim to provide a facility to create and destroy network communication structures.

*/

#include "../../../include/network.h"
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

/* We aim to provide a facility to destroy network task responses. */
int destroy_net_task_response(net_task_response_t *net_task_response) {

    /* Ensure our input parameters are valid. */
    if (!net_task_response) {

        return -1;

    }

    /* Free the memory allocated for the routine return buffer. */
    if (net_task_response->routine_return_buffer) {

        free(net_task_response->routine_return_buffer);

    }

    /* Free the memory allocated for the net task response structure. */
    free(net_task_response);

    /* Return success. */
    return 0;

}

int destroy_net_status(net_status_t *net_status) {

    /* Ensure our input parameters are valid. */
    if (!net_status) {

        return -1;

    }

    /* Free the memory allocated for the net status structure. */
    free(net_status);

    /* Return success. */
    return 0;

}