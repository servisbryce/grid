/*

    We aim to provide a standalone centralized pool for various network structures.

*/

#ifndef NETWORK_H_
#define NETWORK_H_

/* If you are the worker, you are going to receive this structure in serialized form over the socket. You are to deserialize it and you should find the result to be this structure. */
typedef struct net_task_request {

    size_t identifier;
    void *(*routine)(void *routine_vargs_p);
    void *routine_vargs_p;

} net_task_request_t;

/* You are supposed to fill out this structure, serialize this structure, and then send it over the socket. */
typedef struct net_task_response {

    size_t identifier;
    size_t routine_return_buffer_length;
    void *routine_return_buffer_p;

} net_task_response_t;

/* You are to use this whenever responding to a request so the requestee at least knows that you've acknowledged and received the request. You must also serialize this structure and then send it over the socket. */
typedef struct net_status {

    bool status;

} net_status_t;

#endif