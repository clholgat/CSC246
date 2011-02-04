#include <stdio.h>
#include "sock.h"
  
int byte_n;      /* Message length */ 
char buf[ 256 ]; /* Message buffer */
int client_sock; /* Client socket ID */ 

void in_msg();
void msg();
void inp();
void win();
void lose();
void tie();
void quit();

int main( int argc, char *argv[] ) {

  if ( argc != 2 ) {
    printf( "Usage: client SERVER_IP_ADDRESS\n" );
    return 1;
  }

  client_sock = client_connect( argv[ 1 ] );
  
  in_msg(); 
  
  terminate_socket( client_sock );
  return 0; 
}

void in_msg(){
  recv_msg(buf, 256, client_sock);
  
  if(!strncmp(buf, "msg", 3)){
    msg();
  }else if(!strncmp(buf, "inp", 3)){
    inp();
  }else if(!strncmp(buf, "win", 3)){
    win();
  }else if(!strncmp(buf, "lose", 4)){
    lose();
  }else if(!strncmp(buf, "tie", 3)){
    tie();
  }else if(!strncmp(buf, "quit", 4)){
    quit();
  }else{
    printf("This should never happen: %s\n", buf);
  }
}

void msg(){
  recv_msg(buf, 256, client_sock);
  printf("%s", buf);
  in_msg();
}

void inp(){
  printf("Hit or stand? ");
  scanf("%s", buf);
  
  if(!strncmp(buf, "hit", 3)){
    send_msg("hit", client_sock);
    in_msg();
  }else if(!strncmp(buf, "stand", 4)){
    send_msg("stand", client_sock);
    in_msg();
  }else{
    inp();
  }
}
  
void win(){
  printf("Player Wins\n");
  in_msg();
}

void lose(){
  printf("Player Loses\n");
  in_msg();
}

void tie(){
  printf("Tie\n");
  in_msg();
}

void quit(){
  return;
}
