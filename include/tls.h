/*

    We aim to provide accessible functions for server-side and client-side networking in Transport Layer Security.

*/

#ifndef TLS_H_
#define TLS_H_

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

SSL_CTX *create_ssl_server_context(char *certificate_file_path, char *private_key_file_path);
SSL_CTX *create_ssl_client_context(char *certificate_file_path, char *private_key_file_path);

#endif