/* vim:set ft=c ts=2 sw=2 sts=2 et cindent: */
/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MIT
 *
 * Portions created by Alan Antonuk are Copyright (c) 2012-2013
 * Alan Antonuk. All Rights Reserved.
 *
 * Portions created by Mike Steinert are Copyright (c) 2012-2013
 * Mike Steinert. All Rights Reserved.
 *
 * Portions created by VMware are Copyright (c) 2007-2012 VMware, Inc.
 * All Rights Reserved.
 *
 * Portions created by Tony Garnock-Jones are Copyright (c) 2009-2010
 * VMware, Inc. and Tony Garnock-Jones. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ***** END LICENSE BLOCK *****
 */

#include "my_amqps_sendstring.h"

json_t *root; 

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
  
  hostname = "rabbit.nersc.gov";
  //  hostname = "fail-rabbit.nersc.gov";
  port = 5671;
  exchange = "ha-metric";
  routingkey = "ou.das";
  //  routingkey = "fail.ou.das";
  login = "modsuser";
  creds = "3uNne@^z";
  
  json_t *json_test;
  json_error_t json_error;
  
  messagebody = json_dumps(root, 0);
  //fprintf(stderr, "messagebody %s\n",messagebody);
  json_test = json_loads(messagebody, 0, &json_error);
  if(!json_test)
  {
      fprintf(stderr, "error: on line %d: %s\n", json_error.line, json_error.text);
      //return 1;
      return;
  }

  messagebody = json_dumps(json_test,JSON_ENSURE_ASCII|JSON_ESCAPE_SLASH);
  fprintf(stderr,"json_dumps %s\n",messagebody);
  
  conn = amqp_new_connection();
  socket = amqp_ssl_socket_new(conn);
  
  if (!socket) {
    //return die("died while creating SSL/TLS socket");
    die("died while creating SSL/TLS socket");
    return ;
  }

  struct timeval consume_timeout;
  consume_timeout.tv_sec = 1;
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
