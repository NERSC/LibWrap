#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <stdint.h>
#include <amqp_ssl_socket.h>
#include <amqp_framing.h>
#include <jansson.h>

#include "utils.h"

extern json_t *root; 

void send_to_mods();