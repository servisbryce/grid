/* 

    We aim to provide facilities for server-side and client-side networking regardless if we are using transport layer security or not.

*/

#ifndef TCP_H
#define TCP_H

#include <sys/socket.h>
#include <sys/types.h>

struct sockaddr *create_sockaddr(char *address, uint16_t port, unsigned int *sockaddr_length);
int create_socket(struct sockaddr *sockaddr, unsigned int sockaddr_length, unsigned int timeout)

#endif