#include "deck.h"
#include <stdlib.h>
#include <string.h>
#include <time.h> 

int place = 0;

void init_deck(){
  int i = 0;
  
  for(i = 0; i < 52; i++){
    deck[i] = (Card)malloc(sizeof(struct card));
  }
  
  for(i = 0; i < 4; i++){
    deck[i]->value = 11;
    strcpy(deck[i]->rep, "A");
    deck[i+1]->value = 2;
    strcpy(deck[i+1]->rep, "2");
    deck[i+2]->value = 3;
    strcpy(deck[i+2]->rep, "3");
    deck[i+3]->value = 4;
    strcpy(deck[i+3]->rep, "4");
    deck[i+4]->value = 5;
    strcpy(deck[i+4]->rep, "5");
    deck[i+5]->value = 6;
    strcpy(deck[i+5]->rep, "6");
    deck[i+6]->value = 7;
    strcpy(deck[i+6]->rep, "7");
    deck[i+7]->value = 8;
    strcpy(deck[i+7]->rep, "8");
    deck[i+8]->value = 9;
    strcpy(deck[i+8]->rep, "9");
    deck[i+9]->value = 10;
    strcpy(deck[i+9]->rep, "10");
    deck[i+10]->value = 10;
    strcpy(deck[i+10]->rep, "J");
    deck[i+11]->value = 10;
    strcpy(deck[i+11]->rep, "Q");
    deck[i+12]->value = 10;
    strcpy(deck[i+12]->rep, "K");
  }
  shuffle();
}

void shuffle(){
  int i = 0;
  int i_rand;
  Card temp = NULL;
  srand48((unsigned) time(NULL));
  for(i = 0; i < 52; i++){
    i_rand = (int)(drand48()*52.5);
    temp = deck[i];
    deck[i] = deck[i_rand];
    deck[i_rand] = temp;
  }
  place = 0;
}

Card top(){
  Card ret = deck[place];
  place++;
  if(place >= 51){
    shuffle();
  }
  return ret;
}
