/*

    We aim to provide functions for interacting with a thread pool.

*/

#include "../../include/thread_pool.h"
#include <stdlib.h>

/*

    Define the thread worker function which envelopes the logic for each thread.

*/

void *thread_worker(void *thread_worker_vargs_p) {

    if (!thread_worker_vargs_p) {

        return NULL;

    }

    thread_pool_t *thread_pool = (thread_pool_t *) thread_worker_vargs_p;
    while (1) {

        /* If there aren't any thread tasks available, then we should wait for there to be one. */
        pthread_mutex_lock(&(thread_pool->thread_task_head_available_mutex));
        while (!thread_pool->halt && !thread_pool->thread_task_head_available) {

            pthread_cond_wait(&(thread_pool->thread_task_head_condition), &(thread_pool->thread_task_head_available_mutex));

        }

        /* If the thread pool is halted, we should kill this thread immediately. */
        if (thread_pool->halt) {

            break;

        }

        /* If we've reached this section of the worker, we have an available thread task. Mark to the thread pool that we're busy and taking on a task. */
        thread_task_t *thread_task = (thread_task_t *) thread_pool->thread_task_head_available;
        thread_pool->thread_task_head_available = thread_pool->thread_task_head_available->next;
        pthread_mutex_lock(&(thread_pool->thread_inactive_threads_mutex));
        thread_pool->inactive_threads--;
        pthread_mutex_unlock(&(thread_pool->thread_inactive_threads_mutex));
        pthread_mutex_lock(&(thread_pool->thread_active_threads_mutex));
        thread_pool->active_threads++;
        pthread_mutex_unlock(&(thread_pool->thread_active_threads_mutex));

        /* We're going to ensure the thread task isn't null, then we're going to execute it. */
        if (thread_task) {

            /* Execute the function. The function then shall return the same thread task structure but with the return buffer pointer and length set. */
            thread_task = (thread_task_t *) thread_task->routine((void *) thread_task);
            pthread_mutex_lock(&(thread_pool->thread_task_head_completed_mutex));

            /* If the thread task head is marked as null, we'll set it here. Otherwise, push the thread task head. */
            if (!thread_pool->thread_task_head_completed) {

                thread_pool->thread_task_head_completed = thread_task;
                
            } else {

                thread_task_t *thread_task_tail_completed = thread_pool->thread_task_head_completed;
                while (thread_task_tail_completed->next) {

                    thread_task_tail_completed = thread_task_tail_completed->next;

                }

                thread_task_tail_completed->next = thread_task;

            }

            /* Mark this thread as available once the task has been executed successfully. */
            pthread_mutex_unlock(&(thread_pool->thread_task_head_completed_mutex));
            pthread_mutex_lock(&(thread_pool->thread_inactive_threads_mutex));
            pthread_mutex_lock(&(thread_pool->thread_active_threads_mutex));
            thread_pool->inactive_threads++;
            thread_pool->active_threads--;
            if (!thread_pool->halt && thread_pool->active_threads == 0 && !thread_pool->thread_task_head_available) {

                pthread_cond_signal(&(thread_pool->thread_active_threads_condition));

            }

            pthread_mutex_unlock(&(thread_pool->thread_inactive_threads_mutex));
            pthread_mutex_unlock(&(thread_pool->thread_active_threads_mutex));

        }

    }

    /* Tell the thread pool that this thread is being removed from the pool because it's reached the end of its life. */
    pthread_mutex_lock(&(thread_pool->thread_inactive_threads_mutex));
    thread_pool->inactive_threads--;
    pthread_cond_signal(&(thread_pool->thread_active_threads_condition));
    pthread_mutex_unlock(&(thread_pool->thread_inactive_threads_mutex));
    pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));
    return NULL;

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
    pthread_mutex_init(&(thread_pool->thread_task_head_available_mutex), NULL);
    pthread_mutex_init(&(thread_pool->thread_task_head_completed_mutex), NULL);
    pthread_cond_init(&(thread_pool->thread_task_head_condition), NULL);
    pthread_cond_init(&(thread_pool->thread_active_threads_condition), NULL);
    thread_pool->thread_task_head_available = NULL;
    thread_pool->thread_task_head_completed = NULL;
    thread_pool->inactive_threads = inactive_threads;
    thread_pool->active_threads = 0;
    thread_pool->halt = false;

    /* Create the thread workers. */
    for (size_t i = 0; i < inactive_threads; i++) {

        pthread_t thread;
        pthread_create(&thread, NULL, thread_worker, (void *) thread_pool);
        pthread_detach(thread);

    }

    return thread_pool;

}