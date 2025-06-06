/*

    We aim to provide a facility for the program to serialize and deserialize various network structures to allow them to be sent over a socket to a peer.

*/

#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include "network.h"

/* We aim to provide a facility to serialize a network status structure to an encoded string. */
char *serialize_net_status(net_status_t *net_status);

/* We aim to provide a facility to deserialize a serialized network status string. */
net_status_t *deserialize_net_status(char *serialized_net_status);

/* We aim to provide a facility to serialize network task requests.*/
char *serialize_net_task_request(net_task_request_t *net_task_request);

/* We aim to provide a facility to deserialize serialized network task requests. */
net_task_request_t *deserialize_net_task_request(char *serialized_net_task_request);

/* We aim to provide a facility to serialize network deferments. */
char *serialize_net_defer(net_defer_t *net_defer);

/* We aim to provide a facility to deserialize network deferments. */
net_defer_t *deserialize_net_defer(char *serialized_net_defer);

#endif