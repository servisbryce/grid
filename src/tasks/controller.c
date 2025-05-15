/*

    We aim to provide a facility for the program to receive tasks, execute them, and return the results to the user.

*/

#include "../../include/serialization.h"
#include "../../include/thread_pool.h"
#include "../../include/controller.h"
#include "../../include/network.h"
#include <pthread.h>
#include <openssl/err.h>
#include <unistd.h>

/* We aim to provide a facility to handle any incoming connections that we may have. */
void *controller_tls_network_task(void *thread_task_p) {

    /* Cast our pointer into the appropriate type and structure. */
    thread_task_t *thread_task = (thread_task_t *) thread_task_p;

    /* Ensure that the arguments the thread was provide actually exists. */
    if (!thread_task) {

        return (void *) -1;

    }

    /* Retrieve our thread task virtual arguments. */
    controller_tls_network_task_vargs_t *controller_tls_network_task_vargs = (controller_tls_network_task_vargs_t *) thread_task->routine_vargs_p;

    /* Lock the mutex. */
    pthread_mutex_lock(&controller_tls_network_task_vargs->task_list->pending_tasks_mutex);

    /* Check if the pending task list is emptied. */
    if (!controller_tls_network_task_vargs->task_list->pending_tasks) {

        /* We're able to release the mutex here instantly because we aren't reliant on it. Minor performance improvement. */
        pthread_mutex_unlock(&controller_tls_network_task_vargs->task_list->pending_tasks_mutex);

        /* Tell them to go away for sixty seconds. */
        net_defer_t *net_defer = (net_defer_t *) malloc(sizeof(struct net_defer));
        net_defer->defer_time = 60;
        char *response = serialize_net_defer(net_defer);
        free(net_defer);
        SSL_write(controller_tls_network_task_vargs->ssl, response, strlen(response) + 1);
        free(response);

    } else {

        /* Ensure that the mutex is released regardless of the branches.*/
        pthread_mutex_unlock(&controller_tls_network_task_vargs->task_list->pending_tasks_mutex);

    }

    /* If any heap allocated structures exist, we must destroy them or we'll have a major memory leak! */
    if (controller_tls_network_task_vargs->client_sockaddr) {

        free(controller_tls_network_task_vargs->client_sockaddr);

    }

    /* Clean up our SSL connection. */
    if (controller_tls_network_task_vargs->ssl) {

        SSL_shutdown(controller_tls_network_task_vargs->ssl);
        SSL_free(controller_tls_network_task_vargs->ssl);

    }

    /* Finally, close the connection and free the structure. Then, return and let the thread be freed. */
    close(controller_tls_network_task_vargs->client_sockfd);
    free(controller_tls_network_task_vargs);
    free(thread_task);
    return NULL;

}
