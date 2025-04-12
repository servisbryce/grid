#include "../include/thread_pool.h"
#include <stdio.h>

void *print(void *data) {

    thread_task_t *d = (thread_task_t *) data;
    printf("%s\n", (char *) d->routine_vargs_p);
    return data;

}

void main() {

    thread_pool_t *thread_pool = create_thread_pool(4);
    char *msg = "README";
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_assign_task(thread_pool, print, (void *) msg);
    thread_pool_destroy(thread_pool);
    printf("%p\n", thread_pool->thread_task_head_completed);
    printf("%ld\n", thread_pool->active_threads);

}