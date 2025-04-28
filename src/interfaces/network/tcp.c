/* We aim to provide a facility to create a sockaddr structure from an address and port schema. */
struct sockaddr *create_sockaddr(char *address, uint16_t port, unsigned int *sockaddr_length) {

    /* Ensure that our function parameters are valid. */
    if (!address || port <= 0 || !sockaddr_length) {

        return NULL;

    }

    /* Fill out our template for translating the address or domain into an internet protocol address. */
    struct addrinfo *response = NULL;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    /* Convert the address or domain into a addrinfo structure. */
    if (getaddrinfo(address, NULL, &hints, &response) != 0) {

        return NULL;

    }

    /* If we're a regular version 4 address, we should set that size. Vice versa for version 6 addresses. */
    if (response->ai_addr->sa_family == AF_INET) {

        /* Set the length. */
        *sockaddr_length = sizeof(struct sockaddr_in);

        /* Push the completed structure onto the heap. */
        struct sockaddr_in *sockaddr = (struct sockaddr_in *) malloc(*sockaddr_length);
        memcpy(sockaddr, response->ai_addr, *sockaddr_length);
        sockaddr->sin_port = htons(port);

        /* Free the previous structure and return the address of the new structure. */
        freeaddrinfo(response);
        return (struct sockaddr *) sockaddr;
        

    } else if (response->ai_addr->sa_family == AF_INET6) {

        /* Set the length. */
        *sockaddr_length = sizeof(struct sockaddr_in6);

        /* Push the completed structure onto the heap by doing a quick memory copy. */
        struct sockaddr_in6 *sockaddr = (struct sockaddr_in6 *) malloc(*sockaddr_length);
        memcpy(sockaddr, response->ai_addr, *sockaddr_length);
        sockaddr->sin6_port = htons(port);

        /* Free the previous structure and return the address of the new structure. */
        freeaddrinfo(response);
        return (struct sockaddr *) sockaddr;

    }

    /* If there isn't a version indicated, free up the structure and return null to indicate something went wrong. */
    freeaddrinfo(response);
    return NULL;

}

int create_socket(struct sockaddr *sockaddr, unsigned int sockaddr_length, long timeout) {

    /* Ensure that our function parameters are valid. */
    if (!sockaddr || sockaddr_length <= 0) {

        return -1;

    }

    /* Create a socket. */
    if ()

}