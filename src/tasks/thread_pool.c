/*

    We aim to provide functions for interacting with a thread pool.

*/

#include "../../include/thread_pool.h"
#include <stdlib.h>

/* Define the thread worker function which envelopes the logic for each thread. */
void *thread_worker(void *thread_worker_vargs_p) {

    if (!thread_worker_vargs_p) {

        return NULL;

    }

    thread_pool_t *thread_pool = (thread_pool_t *) thread_worker_vargs_p;
    while (1) {

        /* If there aren't any thread tasks available, then we should wait for there to be one. */
        pthread_mutex_lock(&(thread_pool->thread_task_head_available_mutex));
        while (!thread_pool->halt && !thread_pool->thread_task_head_available) {

            pthread_cond_wait(&(thread_pool->thread_task_head_available_condition), &(thread_pool->thread_task_head_available_mutex));

        }

        /* If the thread pool is halted, we should kill this thread immediately. */
        if (thread_pool->halt) {

            break;

        }

        /* If we've reached this section of the worker, we have an available thread task. Mark to the thread pool that we're busy and taking on a task. */
        thread_task_t *thread_task = (thread_task_t *) thread_pool->thread_task_head_available;
        thread_pool->thread_task_head_available = thread_pool->thread_task_head_available->next;
        pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));
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

            /* If the thread task head is marked as null, we'll set it here. Otherwise, push the thread task head. */
            pthread_mutex_lock(&(thread_pool->thread_task_head_completed_mutex));
            if (!thread_pool->thread_task_head_completed) {

                thread_pool->thread_task_head_completed = thread_task;
                
            } else {

                thread_task_t *thread_task_tail_completed = thread_pool->thread_task_head_completed;
                while (thread_task_tail_completed->next) {

                    thread_task_tail_completed = thread_task_tail_completed->next;

                }
        
                thread_task_tail_completed->next = thread_task;
                thread_task->next = NULL;

            }

            pthread_mutex_unlock(&(thread_pool->thread_task_head_completed_mutex));
            pthread_cond_broadcast(&(thread_pool->thread_task_head_completed_condition));

        }

        /* Mark this thread as available once the task has been executed successfully. */
        pthread_mutex_lock(&(thread_pool->thread_inactive_threads_mutex));
        thread_pool->inactive_threads++;
        pthread_mutex_unlock(&(thread_pool->thread_inactive_threads_mutex));
        pthread_mutex_lock(&(thread_pool->thread_active_threads_mutex));
        thread_pool->active_threads--;
        pthread_mutex_unlock(&(thread_pool->thread_active_threads_mutex));
        pthread_mutex_lock(&(thread_pool->thread_task_head_available_mutex));
        if (!thread_pool->halt && thread_pool->active_threads == 0 && !thread_pool->thread_task_head_available) {
        
            pthread_cond_signal(&(thread_pool->thread_active_threads_condition));
        
        }
        
        pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));

    }

    /* Tell the thread pool that this thread is being removed from the pool because it's reached the end of its life. */
    pthread_mutex_lock(&(thread_pool->thread_inactive_threads_mutex));
    thread_pool->inactive_threads--;
    pthread_mutex_unlock(&(thread_pool->thread_inactive_threads_mutex));
    pthread_cond_signal(&(thread_pool->thread_active_threads_condition));
    pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));
    return NULL;

}

/* We aim to create the structure defined in the corresponding thread pool and threads. */
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
    pthread_cond_init(&(thread_pool->thread_task_head_available_condition), NULL);
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

/* We aim to create a function that allows the program to insert tasks into the thread pool available task queue. */
thread_task_t *thread_pool_assign_task(thread_pool_t *thread_pool, void *(*routine)(void *routine_vargs_p), void *routine_vargs_p) {

    /* Ensure that our function parameters are valid. */
    if (!thread_pool || thread_pool->halt || !routine) {

        return NULL;

    }

    /* Create and populate the thread task structure. */
    thread_pool->tasks_assigned++;
    thread_task_t *thread_task = (thread_task_t *) malloc(sizeof(thread_task_t));
    thread_task->next = NULL;
    thread_task->routine = routine;
    thread_task->identifier = thread_pool->tasks_assigned;
    thread_task->routine_vargs_p = routine_vargs_p;
    pthread_mutex_lock(&(thread_pool->thread_task_head_available_mutex));

    /* Find the tail of the available task list and make the thread task the new tail. */
    if (!thread_pool->thread_task_head_available) {

        thread_pool->thread_task_head_available = thread_task;

    } else {

        thread_task_t *thread_task_tail_available = thread_pool->thread_task_head_available;
        while (thread_task_tail_available->next) {

            thread_task_tail_available = thread_task_tail_available->next;

        }

        thread_task_tail_available->next = thread_task;

    }

    /* Alert the program that a new task is available. Then, return the structure. */
    pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));
    pthread_cond_broadcast(&(thread_pool->thread_task_head_available_condition));
    return thread_task;

}

/* We aim to create a function that locks the program until all available tasks are completed. */
int thread_pool_wait(thread_pool_t *thread_pool) {

    /* Ensure all function parameters are valid. */
    if (!thread_pool) {

        return -1;

    }

    /* Deadlock the program while we wait for there to be no more available tasks to complete. */
    pthread_mutex_lock(&(thread_pool->thread_task_head_available_mutex));
    while (1) {

        if (thread_pool->thread_task_head_available || (!thread_pool->halt && thread_pool->active_threads != 0) || (thread_pool->halt && (thread_pool->active_threads + thread_pool->inactive_threads) != 0)) {

            pthread_cond_wait(&(thread_pool->thread_active_threads_condition), &(thread_pool->thread_task_head_available_mutex));

        } else {

            break;

        }

    }

    /* Unlock the program and return a success code. */
    pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));
    return 0;

}

/* This function creates a facility to destroy the thread pool structure in a memory-safe manner. */
int thread_pool_destroy(thread_pool_t *thread_pool) {

    /* Ensure our function parameters are valid. */
    if (!thread_pool) {

        return -1;

    }

    /* Search through all of our available and completed tasks, unallocate them, and destroy them. */
    pthread_mutex_lock(&(thread_pool->thread_task_head_available_mutex));
    thread_task_t *previous_available_task = NULL;
    thread_task_t *current_available_task = thread_pool->thread_task_head_available;
    if (previous_available_task) {

        while (previous_available_task->next) {

            previous_available_task = current_available_task;
            current_available_task = current_available_task->next;
            free(previous_available_task);

        }

        free(current_available_task);

    }

    thread_pool->thread_task_head_available = NULL;
    pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));
    pthread_mutex_lock(&(thread_pool->thread_task_head_completed_mutex));
    thread_task_t *previous_completed_task = NULL;
    thread_task_t *current_completed_task = thread_pool->thread_task_head_completed;
    if (current_completed_task) {

        while (current_completed_task->next) {

            previous_completed_task = current_completed_task;
            current_completed_task = current_completed_task->next;
            free(previous_completed_task);

        }

        free(current_completed_task);

    }

    thread_pool->thread_task_head_completed = NULL;
    pthread_mutex_unlock(&(thread_pool->thread_task_head_completed_mutex));

    /* Halt the thread pool and alert the threads that they've been ordered to terminate.*/
    thread_pool->halt = true;
    pthread_cond_broadcast(&(thread_pool->thread_task_head_available_condition));
    pthread_mutex_unlock(&(thread_pool->thread_task_head_available_mutex));

    /* Wait for the threads to terminate. */
    if (thread_pool_wait(thread_pool) != 0) {

        return -1;

    }

    /* Once the threads have terminated and the thread pool is inactive, immediately destroy all conditions and mutexes used by the thread pool. Then, unallocate the thread pool and return a success code. */
    pthread_mutex_destroy(&(thread_pool->thread_inactive_threads_mutex));
    pthread_mutex_destroy(&(thread_pool->thread_active_threads_mutex));
    pthread_mutex_destroy(&(thread_pool->thread_task_head_available_mutex));
    pthread_mutex_destroy(&(thread_pool->thread_task_head_completed_mutex));
    pthread_cond_destroy(&(thread_pool->thread_task_head_available_condition));
    pthread_cond_destroy(&(thread_pool->thread_active_threads_condition));
    free(thread_pool);
    return 0;

}

