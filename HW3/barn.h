    
typedef struct command{
    int time;
    char animal[256];
    char action[256];
    struct command *next;
} *Command;

typedef struct animal{
    char animal[256];
    char location[256];
    int scbit;
    struct animal *next;
} *Animal;

typedef struct bq{
    int index;
    struct bq *next;
} *BQ;

Animal Ahead = NULL;
Animal Atail = NULL;

Command Chead = NULL;
Command Ctail = NULL;

void schedule(int numStalls);
Animal find(char *name);
void removeB(Animal animal, BQ *BqHead, BQ *BqTail, Animal barn[], int numStalls);
void print(int time, char *action, char *animal, Animal barn[], BQ bqHead, int numStalls);
