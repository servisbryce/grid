/*

    We aim to provide a facility for the program to receive tasks, execute them, and return the results to the user.

*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <openssl/ssl.h>
#include <pthread.h>

/* We're going to reuse the thread task structure here because it doesn't make sense to reinvent the wheel and create a different structure with the same composition but a different purpose. */
typedef struct task_list {

	pthread_mutex_t pending_tasks_mutex;
	thread_task_t *pending_tasks;

} task_list_t;

/* Multiple processes will be using the data provided in this linked list.*/
typedef struct controller_tls_network_task_vargs {

	struct task_list *task_list;
	struct sockaddr_in *client_sockaddr;
	SSL *ssl;
	int client_sockfd;

} controller_tls_network_task_vargs_t;

/* We aim to provide a facility to handle controller network tasks on the server-side. */
void *controller_tls_network_task(void *controller_network_task_vargs_p);

#endif
