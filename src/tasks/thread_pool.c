/*

    We aim to provide functions for interacting with a thread pool.

*/

#include "../../include/thread_pool.h"
#include <stdlib.h>

/*

    Define the thread worker function which envelopes the logic for each thread.

*/

void *thread_worker(void *thread_worker_vargs_p) {



}

/*

    We aim to create the structure defined in the corresponding thread pool and threads.

*/

thread_pool_t *create_thread_pool(size_t inactive_threads) {

    /* Ensure that our function parameters are valid. */
    if (inactive_threads < 1) {

        return NULL;

    }

    /* Setup the thread pool structure. */
    thread_pool_t *thread_pool = (thread_pool_t *) malloc(sizeof(thread_pool_t));
    pthread_mutex_init(&(thread_pool->thread_inactive_threads_mutex), NULL);
    pthread_mutex_init(&(thread_pool->thread_active_threads_mutex), NULL);
    pthread_mutex_init(&(thread_pool->thread_task_head_mutex), NULL);
    pthread_cond_init(&(thread_pool.->thread_task_head_condition), NULL);
    pthread_cond_init(&(thread_pool->thread_active_threads_condition), NULL);
    thread_pool->thread_task_head = NULL;
    thread_pool->inactive_threads = inactive_threads;
    thread_pool->active_threads = 0;
    thread_pool->halt = false;
    for (size_t i = 0; i < inactive_threads; i++) {

        pthread_t thread;
        pthread_create(&thread, NULL, NULL, NULL);
        pthread_detach(&thread);

    }

    return thread_pool;

}