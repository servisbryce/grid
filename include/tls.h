/*

    We aim to provide accessible functions for server-side and client-side networking in transport layer security.

*/

#ifndef TLS_H_
#define TLS_H_

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

/* This function creates a facility where the user may create a secure socket layer context intended for a server from certificate and private key files. */
SSL_CTX *create_ssl_server_context(char *certificate_file_path, char *private_key_file_path);

/* This function creates a facility where the user may create a secure socket layer context intended for a client from certificate and private key files. */
SSL_CTX *create_ssl_client_context(char *certificate_file_path, char *private_key_file_path);

#endif