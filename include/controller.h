/*

    We aim to provide a facility for the program to receive tasks, execute them, and return the results to the user.

*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct controller_network_task {

	struct sockaddr_in *client_sockaddr;
	SSL *ssl;
	int client_sockfd;

} controller_network_task_t

#endif
