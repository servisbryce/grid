/*

    We aim to provide a standalone centralized pool for various network structures.

*/

#ifndef NETWORK_H_
#define NETWORK_H_

#include "stddef.h"
#include "stdbool.h"

/* If you are the controller, you will send this to a client to request them to execute a task. */
typedef struct net_task_request {

    size_t identifier;
    size_t routine_file_length;
    void *routine_file;
    size_t routine_arguments_length;
    void *routine_arguments;

} net_task_request_t;

/* You are supposed to fill out this structure, serialize this structure, and then send it over the socket. */
typedef struct net_task_response {

    size_t identifier;
    size_t routine_return_buffer_length;
    void *routine_return_buffer;

} net_task_response_t;

/* You are to use this whenever responding to a request so the requestee at least knows that you've acknowledged and received the request. You must also serialize this structure and then send it over the socket. */
typedef struct net_status {

    bool status;

} net_status_t;

/* You are to use this to tell a client to go away for a specified amount of time because there aren't any tasks for them to do. */
typedef struct net_defer {

    int defer_time;

} net_defer_t;

/* We aim to provide facilities to destroy various structures. */
int destroy_net_task_request(net_task_request_t *net_task_request);
int destroy_net_task_response(net_task_response_t *net_task_response);
int destroy_net_status(net_status_t *net_status);

#endif