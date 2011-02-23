#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sock.h"
#include "deck.h"

Card dealer[20];
int dnumcards = 0;
int dnum = 0;

Card player[20];
int pnumcards = 0;
int pnum = 0;

char *String;
char buf[ 256 ]; /* Message buffer */
int client_sock; /* Client socket ID */ 
int server_sock; /* Server socket ID */

void setup();
void play();

int main() {
  String = (char *)malloc(sizeof(char)*256);
  server_sock = server_connect();

  client_sock = wait_for_client( server_sock, buf, 256 );
  printf( "Player connects @ %s\n", buf );

  setup();
  play();
  
  //send_msg("msg", client_sock);
  //sprintf(String, "dealer: %d, client %d\n", dnum, pnum);
  //send_msg(String, client_sock);
  send_msg("msg", client_sock);
  send_msg( "The game has ended\n", client_sock );
  send_msg("quit", client_sock);

  terminate_socket( client_sock );
  terminate_socket( server_sock );

  return 0; 
}

void setup(){
  init_deck();
  shuffle();
  
  dealer[0] = top();
  dealer[1] = top();
  dnumcards = 2;
  dnum = dealer[0]->value + dealer[1]->value;
  
  player[0] = top();
  player[1] = top();
  pnumcards = 2;
  pnum = player[0]->value + player[1]->value;
  
  send_msg("msg", client_sock);
  sprintf(String, "Dealer: X %s\n", dealer[1]->rep);
  send_msg(String, client_sock);
  
  send_msg("msg", client_sock);
  sprintf(String, "Player: %s %s\n", player[0]->rep, player[1]->rep);
  send_msg(String, client_sock);
    
}
  
void play(){
  if(dnum > 21){
    send_msg("win", client_sock);
    return;
  }
  
  if(pnum > 21){
    send_msg("lose", client_sock);
    return;
  }
  
  send_msg("inp", client_sock);
  int i = 0;
  while(recv_msg(buf, 256, client_sock) != 0){
    if(!strncmp("hit", buf, 3)){
      player[pnumcards] = top();
      pnum += player[pnumcards]->value;
      pnumcards++;
      send_msg("msg", client_sock);
      sprintf(String, "Player:");
      for(i = 0; i < pnumcards; i++){
        strcat(String, " ");
        strcat(String, player[i]->rep);
      }
      strcat(String, "\n");
      send_msg(String, client_sock);
      if(pnum > 21){
        send_msg("lose", client_sock);
        return;
      }else{
        send_msg("inp", client_sock);
      }
    }else{
      while(dnum < 17){
        dealer[dnumcards] = top();
        dnum += dealer[dnumcards]->value;
        dnumcards++;
        send_msg("msg", client_sock);
        sprintf(String, "Dealer:");
        for(i = 0; i < dnumcards; i++){
          strcat(String, " ");
          strcat(String, dealer[i]->rep);
        }
        strcat(String, "\n");
        send_msg(String, client_sock);
      }
      
      if(dnum > pnum){
        send_msg("lose", client_sock);
        return;
      }else if(dnum < pnum){
        send_msg("win", client_sock);
        return;
      }else{
        send_msg("tie", client_sock);
        return;
      }
    }
  }
}
    
