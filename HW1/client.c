#include <stdio.h>
#include "sock.h"

int main( int argc, char *argv[] ) {
  int byte_n;      /* Message length */ 
  char buf[ 256 ]; /* Message buffer */
  int client_sock; /* Client socket ID */ 


  if ( argc != 2 ) {
    printf( "Usage: client SERVER_IP_ADDRESS\n" );
    return 1;
  }

  client_sock = client_connect( argv[ 1 ] );
  
  recv_msg(buf, 256, client_sock);
  printf(buf);
  recv_msg(buf, 256, client_sock);
  printf(buf);
  printf("hit or stand? ");
  scanf("%s", &buf);
  
  while(1){
    if(!strncmp(buf,"hit", 3)){
      send_msg("hit");
      recv_msg(buf, 256, client_sock);
      printf(buf);
      printf("hit or stand? ");
      scanf("%s", &buf);
    }else if(!strncmp(buf, "stand")){
      send_msg("stand", client_sock);
      while(recv_msg(buf, 256, client_sock != 0){
        if(!strncmp(
    
  while(recv_msg(buf, 256, client_sock != 0){
    if(!strncmp(buf, "msg", 3)){
      printf(buf);
      

  recv_msg( buf, 256, client_sock );
  printf( "%s\n", buf ); 
  send_msg( "Hello from client", client_sock );

  terminate_socket( client_sock );
  return 0; 
}
