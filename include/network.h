#ifndef NETWORK_H_
#define NETWORK_H_

typedef struct net_task_request {

    size_t identifier;
    void *(*routine)(void *routine_vargs_p);
    void *routine_vargs_p;

} net_task_request_t;

typedef struct net_task_response {

    size_t identifier;
    size_t routine_return_buffer_length;
    void *routine_return_buffer_p;

} net_task_response_t;

typedef struct net_status {

    bool status;

} net_status_t;

#endif