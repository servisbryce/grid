/*

    We aim to provide accessible functions for server-side and client-side networking with transport layer security.

*/

#include "../../../include/tls.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <netdb.h>
#include <err.h>

/* Create a secure socket layer context intended to be used on the server-side. */
SSL_CTX *create_ssl_server_context(char *certificate_file_path, char *private_key_file_path) {

    /* Prevent any null parameters from being passed. */
    if (!certificate_file_path || !private_key_file_path) {

        return NULL;

    }

    /* Create the secure socket layer context. */
    SSL_CTX *ssl_context = NULL;
    if (!(ssl_context = SSL_CTX_new(TLS_server_method()))) {

        errx(EXIT_FAILURE, "There was an unexpected exception while trying to create a secure transport layer security context for the server.");

    }

    /* Set the minimum protocol version to oldest non-deprecated version. */
    if (!SSL_CTX_set_min_proto_version(ssl_context, TLS1_2_VERSION)) {

        SSL_CTX_free(ssl_context);
        errx(EXIT_FAILURE, "There was an unexpected version while trying to set a minimum transport layer security version.");

    }

    /* Set recommended options. */
    long ssl_options = SSL_OP_IGNORE_UNEXPECTED_EOF || SSL_OP_NO_RENEGOTIATION || SSL_OP_CIPHER_SERVER_PREFERENCE;
    SSL_CTX_set_options(ssl_context, ssl_options);

    /* Assign the context a certificate file. */
    if (SSL_CTX_use_certificate_chain_file(ssl_context, certificate_file_path) <= 0) {

        SSL_CTX_free(ssl_context);
        errx(EXIT_FAILURE, "There was an error while trying to use the certificate file provided.");

    }

    /* Assign the context a private key file. */
    if (SSL_CTX_use_PrivateKey_file(ssl_context, private_key_file_path, SSL_FILETYPE_PEM) <= 0) {

        SSL_CTX_free(ssl_context);
        errx(EXIT_FAILURE, "There was an error while trying to use the private key file provided.");

    }

    /* Assign various parameters for the secure socket layer session cache. */
    size_t session_cache_length = 32768;
    long session_timeout_duration = 3600;
    int cache_id = 1;

    /* Setup a secure socket layer session cache to reduce latency during handshakes. */
    SSL_CTX_set_session_id_context(ssl_context, (void *) cache_id, sizeof(cache_id));
    SSL_CTX_set_session_cache_mode(ssl_context, SSL_SESS_CACHE_SERVER);
    SSL_CTX_sess_set_cache_size(ssl_context, session_cache_length);
    SSL_CTX_set_timeout(ssl_context, session_timeout_duration);

    /* Disable mutual transport layer security certificate-based authentication on the server-side. */
    SSL_CTX_set_verify(ssl_context, SSL_VERIFY_NONE, NULL);

    /* Return the completed secure socket layer structure. */
    return ssl_context;

}

/* Create a secure socket layer context intended to be used on the server-side. */
SSL_CTX *create_ssl_client_context(char *certificate_file_path, char *private_key_file_path) {

    /* Prevent any null parameters from being passed. */
    if (!certificate_file_path || !private_key_file_path) {

        return NULL;

    }

    /* Create the secure socket layer context. */
    SSL_CTX *ssl_context = NULL;
    if (!(ssl_context = SSL_CTX_new(TLS_client_method()))) {

        errx(EXIT_FAILURE, "There was an unexpected exception while trying to create a secure transport layer security context for the server.");

    }

    /* Enforce secure socket layer verification of peers. */
    SSL_CTX_set_verify(ssl_context, SSL_VERIFY_PEER, NULL);

    /* Enforce the default trusted certificate store. */
    if (!SSL_CTX_set_default_verify_paths(ssl_context)) {

        SSL_CTX_free(ssl_context);
        errx(EXIT_FAILURE, "There was an unexpected exception while trying to assign the trusted certificate store.");

    }

    /* Enforce a minimum non-deprecated secure socket layer version. */
    if (!SSL_CTX_set_min_proto_version(ssl_context, TLS1_2_VERSION)) {

        SSL_CTX_free(ssl_context);
        errx(EXIT_FAILURE, "There was an unexpected exception while trying to enforce a minimum transport layer security version.");

    }

    /* Return the completed secure socket layer structure. */
    return ssl_context;
    
}

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