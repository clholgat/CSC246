#include <stdio.h>
#include "sock.h"

int main() {
  char buf[ 256 ]; /* Message buffer */
  int client_sock; /* Client socket ID */ 
  int server_sock; /* Server socket ID */


  server_sock = server_connect();

  client_sock = wait_for_client( server_sock, buf, 256 );
  printf( "Client @ %s\n", buf );

  send_msg( "Welcome to the server", client_sock );
  recv_msg( buf, 256, client_sock );
  printf( "%s\n", buf ); 

  terminate_socket( client_sock );
  terminate_socket( server_sock );

  return 0; 
}
