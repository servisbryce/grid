/*

    We aim to provide accessible functions for server-side and client-side networking with transport layer security.

*/

#include "../../include/tls.h"
#include <err.h>

/*

    Create a secure socket layer context intended to be used on the server-side.

*/

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

/*

    Create a secure socket layer context intended to be used on the server-side.

*/

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