#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_ssl_socket.h>
#include <amqp_framing.h>
#include <jansson.h>
#include <unistd.h>

#include "utils.h"
#include "main.h"

int create_count, open_count;
struct log_info job_log;
//char* host;
//void send_to_mods(const char * file_operation, const int ismpi, const char *loctime,
// 				const int uid, struct log_info job_log, const int count)
void send_to_mods()
{
  char const *hostname;
  int port, status;
  char const *exchange;
  char const *routingkey;
  char *messagebody;
  char const *login;
  char const *creds;
  amqp_socket_t *socket;
  amqp_connection_state_t conn;
  char const *message_text;
  json_t *json_test;
  json_error_t json_error;
  hostname = "rabbit.nersc.gov";
  //  hostname = "fail-rabbit.nersc.gov";
  port = 5671;
  exchange = "ha-metric";
  routingkey = "ou.das";
  //  routingkey = "fail.ou.das";
  login = "modsuser";
  creds = "3uNne@^z";
   
  json_t *root = json_object();
  json_object_set_new( root, "category", json_string("MODS") );
  json_object_set_new( root, "name", json_string("gotchaio-tracer") );
  json_object_set_new( root, "uid", json_integer(job_log.uid) );
  json_object_set_new(root, "ismpi", json_integer(job_log.ismpi));
  json_object_set_new(root, "first HDF5 API time", json_string(job_log.first_hdf5api_time));
  json_object_set_new(root, "nersc host", json_string(job_log.host));
  json_object_set_new(root, "user", json_string(job_log.user));
  json_object_set_new(root, "slurm number of nodes", json_string(job_log.slurm_job_num_nodes));
  json_object_set_new(root, "slurm job account", json_string(job_log.slurm_job_account));
  json_object_set_new(root, "nodetype", json_string(job_log.nodetype));
  //json_object_set_new(root, "is_compute", json_integer(job_log.is_compute));
  json_object_set_new(root, "count create", json_integer(create_count));
  json_object_set_new(root, "count open", json_integer(open_count));
  
  messagebody = json_dumps(root, 0);
  //printf("messagebody %s\n",messagebody);
  json_test = json_loads(messagebody, 0, &json_error);
  if(!json_test)
    {
      fprintf(stderr, "error: on line %d: %s\n", json_error.line, json_error.text);
      //return 1;
      return;
    }


  messagebody = json_dumps(json_test,JSON_ENSURE_ASCII|JSON_ESCAPE_SLASH);
  // printf("json_dumps %s\n",messagebody);

  conn = amqp_new_connection();

  socket = amqp_ssl_socket_new(conn);
  if (!socket) {
    //return die("died while creating SSL/TLS socket");
    die("died while creating SSL/TLS socket");
    return ;
  }

  struct timeval consume_timeout;
  consume_timeout.tv_sec = 5;
  consume_timeout.tv_usec = 0;

  amqp_ssl_socket_set_verify_peer(socket, 0);
  amqp_ssl_socket_set_verify_hostname(socket, 0);

  status = amqp_socket_open_noblock(socket, hostname, port, &consume_timeout);
  if (status) {
    //return die("died while opening connection hostname %s port %d status %d",hostname,port,status);
    die("died while opening connection hostname %s port %d status %d",hostname,port,status);
    return ;
  }

  /* Set all the timeouts! */
  amqp_set_handshake_timeout(conn, &consume_timeout);
  amqp_set_rpc_timeout(conn, &consume_timeout);

  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, login, creds),
                    "died while logging in");
  amqp_channel_open(conn, 1);
  if (die_on_amqp_error(amqp_get_rpc_reply(conn), "died while opening channel")==-1){
	//return -1;
        return ;
  }
  {
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */
    if (die_on_error(amqp_basic_publish(conn,
                                    1,
                                    amqp_cstring_bytes(exchange),
                                    amqp_cstring_bytes(routingkey),
                                    1,
                                    0,
                                    &props,
				    amqp_cstring_bytes(messagebody)),
                 "died while publishing")==-1){
     	//return -1;
	return ;
     }
  }

  if (die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "died while closing channel")==-1){
	//return -1;
	return ;
  }
  if (die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "died while closing connection")==-1){
	//return -1;
	return ;
  }
  if (die_on_error(amqp_destroy_connection(conn), "died while ending connection")==-1){
	//return -1;
	return ;
  }
  return ;
}
