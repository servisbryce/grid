/*

    We aim to provide structures and functions to interact with the thread pool.

*/

#include <pthread.h>

typedef struct thread_task {

    struct thread_task *next;
    void *(*routine)(void *routine_vargs_p);
    void *routine_vargs_p;

} thread_task_t;

typedef struct thread_pool {

    pthread_mutex_t thread_inactive_threads_mutex;
    pthread_mutex_t thread_active_threads_mutex;
    pthread_mutex_t thread_task_head_mutex;
    pthread_cond_t thread_task_head_condition;
    pthread_cond_t thread_active_threads_mutex;
    thread_task_t *thread_task_head;
    size_t inactive_threads;
    size_t active_threads;

} thread_pool_t;