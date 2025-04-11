/*

    We aim to provide structures and functions to interact with the thread pool.

*/

#include <pthread.h>
#include <stdbool.h>

/*

    This structure defines a task which will be plucked from the thread pool and executed.

*/

typedef struct thread_task {

    struct thread_task *next;

    /* Here we define the routine call and arguments. */
    void *(*routine)(void *routine_vargs_p);
    void *routine_vargs_p;

    /* Here we define the buffer where the return value from a routine is stored. */
    size_t routine_return_buffer_length;
    void *routine_return_buffer_p;

} thread_task_t;

/*

    This structure defines a thread pool. The various mutexes and conditions are defined here along with the statuses of the thread pool.

*/

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
    bool halt;

} thread_pool_t;

/* This function creates a thread pool structure and creates a specified set of thread workers. */
thread_pool_t *create_thread_pool(size_t inactive_threads);
thread_task_t *thread_pool_assign_task(thread_pool_t *thread_pool, void *(*routine)(void *routine_vargs_p), void *routine_vargs_p);