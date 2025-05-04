/*

    We aim to provide a facility for the program to receive tasks, execute them, and return the results to the user.

*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <openssl/ssl.h>

typedef struct controller_network_task_vargs {

	struct sockaddr_in *client_sockaddr;
	SSL *ssl;
	int client_sockfd;

} controller_network_task_vargs_t;

/* We aim to provide a facility to handle controller network tasks on the server-side. */
void *controller_network_task(void *controller_network_task_vargs_p);

#endif
