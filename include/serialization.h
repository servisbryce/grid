/*

    We aim to provide a facility for the program to serialize and deserialize various network structures to allow them to be sent over a socket to a peer.

*/

#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include "network.h"

char *serialize_net_status(net_status_t *net_status);
net_status_t *deserialize_net_status(char *serialized_net_status);

#endif