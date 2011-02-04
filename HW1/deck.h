#ifndef DECK
#define DECK

typedef struct card{
  int value;
  char rep[3];
} *Card;

Card deck[52];

void init_deck();
void shuffle();
Card top();


#endif
