/*

    We aim to provide structures and functions to interact with the thread pool.

*/

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdbool.h>

/* This structure defines a task which will be plucked from the thread pool and executed. */
typedef struct thread_task {

    struct thread_task *next;
    size_t identifier;

    /* Here we define the routine call and arguments. */
    void *(*routine)(void *routine_vargs_p);
    void *routine_vargs_p;

    /* Here we define the buffer where the return value from a routine is stored. */
    size_t routine_return_buffer_length;
    void *routine_return_buffer_p;

} thread_task_t;

/* This structure defines a thread pool. The various mutexes and conditions are defined here along with the statuses of the thread pool. */
typedef struct thread_pool {

    pthread_mutex_t thread_inactive_threads_mutex;
    pthread_mutex_t thread_active_threads_mutex;
    pthread_mutex_t thread_task_head_available_mutex;
    pthread_mutex_t thread_task_head_completed_mutex;
    pthread_cond_t thread_task_head_condition;
    pthread_cond_t thread_active_threads_condition;
    thread_task_t *thread_task_head_available;
    thread_task_t *thread_task_head_completed;
    size_t inactive_threads;
    size_t active_threads;
    size_t tasks_assigned;
    bool halt;

} thread_pool_t;

/* This function creates a thread pool structure and creates a specified set of thread workers. */
thread_pool_t *create_thread_pool(size_t inactive_threads);

/* This function creates a facility for the program to insert tasks into the available task queue for a thread pool. */
thread_task_t *thread_pool_assign_task(thread_pool_t *thread_pool, void *(*routine)(void *routine_vargs_p), void *routine_vargs_p);

/* This function creates a facility for the program to wait for all threads in the thread pool to become inactive. */
int thread_pool_wait(thread_pool_t *thread_pool);

/* This function creates a facility for the program to destroy the thread pool in a memory-safe manner. */
int thread_pool_destroy(thread_pool_t *thread_pool);

#endif