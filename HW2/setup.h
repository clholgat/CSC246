
typedef struct rider{
  int time;
  char id[10];
  int from;
  int to;
  struct rider *next;
} *Rider, rid;


void printState(int time, int curFloor, int targetFloor, int numFloors, Rider waiting[], Rider elevator);
  
void runElevator(int floors, Rider first, int numRiders);

int chooseNext(int floors, Rider waiting[], int curFloor, Rider elevator);
