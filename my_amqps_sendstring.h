#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <stdint.h>
#include <amqp_ssl_socket.h>
#include <amqp_framing.h>
#include <jansson.h>

#include "utils.h"

extern int open_count, create_count, close_count, dread_count;
extern struct log_info job_log;
void send_to_mods();
