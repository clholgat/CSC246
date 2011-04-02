#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "barn.h"

int main(int argc, char *argv[]){
    FILE *fp = fopen(argv[1], "r");
    if(!fp){
        fprintf(stderr, "Could not find file\n");
        return 1;
    }
    
    int numStalls = 0;
    int read = 0;
    fscanf(fp, "%d", &numStalls);
    char *animal = (char *)malloc(sizeof(char)*256);
    char *action = (char *)malloc(sizeof(char)*256);
    int time = 0;
    while(!feof(fp)){
        read = fscanf(fp, "%d %s %s", &time, animal, action);
        if(read < 3){
            break;
        }
        if(Chead == NULL){
            Chead = (Command)malloc(sizeof(struct command));
            Chead->time = time;
            strncpy(Chead->animal, animal, 255);
            strncpy(Chead->action, action, 255);
            Ctail = Chead;
        }else{
            Command next = (Command)malloc(sizeof(struct command));
            next->time = time;
            strncpy(next->animal, animal, 255);
            strncpy(next->action, action, 255);
            next->next = NULL;
            Ctail->next = next;
            Ctail = next;
        }
    }
    Command iter = Chead;
    while(iter != NULL){
        iter = iter->next;
    }
    
    schedule(numStalls);
    return 1;
}

void schedule(int numStalls){
    Animal barn[numStalls];
    for(int i = 0; i < numStalls; i++){
        barn[i] = NULL;
    }
    BQ BqHead = NULL;
    BQ BqTail = NULL;
    Animal animal = NULL;
    Animal removeAnimal = NULL;
    int animalIndex = 0;
    Command Citer = Chead;
    int time = 1;
    int place = 0;
    while(Citer != NULL){
        if(Citer->time > time){
            print(time, "", "", barn, BqHead, numStalls);
            time++;
            continue;
        }
        if(!strcmp(Citer->action, "retrieve")){
            animal = find(Citer->animal);
            strncpy(animal->location, "long-term pasture", 255);
            print(time, "retrieve", animal->animal, barn, BqHead, numStalls);
            time++;
            Citer = Citer->next;
        }else if(!strcmp(Citer->action, "return")){
            animal = find(Citer->animal);
            strncpy(animal->location, "in transit", 255);
            removeB(animal, &BqHead, &BqTail, barn, numStalls);
            print(time, "return", animal->animal, barn, BqHead, numStalls);
            time++;
            if(Citer->next != NULL && Citer->next->time <= time){
                print(time, "blocked on transit", "", barn, BqHead, numStalls);
            }else{
                print(time, "", "", barn, BqHead, numStalls);
            }
            time++;
            strncpy(animal->location, "long-term pasture", 255);
            Citer = Citer->next;
        }else{//tend block
            animal = find(Citer->animal);
            strncpy(animal->location, "in transit", 255);
            place = 0;
            //check if in barn
            for(int i = 0; i < numStalls; i++){
                if(barn[i] == animal){
                    place = 2;
                    animal->scbit = 1;
                    break;
                }
            }
            //not in barn
            if(place != 2){
                for(int i = 0; i < numStalls; i++){
                    //empty spot
                    if(barn[i] == NULL){
                        animalIndex = i;
                        animal->scbit = 1;
                        place = 1;
                        break;
                    }
                }
            //in barn
            }else{
                print(time, "tend", animal->animal, barn, BqHead, numStalls);
                time++;
            }
            //empty spot
            if(place == 1){
                print(time, "tend", animal->animal, barn, BqHead, numStalls);
                time++;
                if(Citer->next != NULL && Citer->next->time <= time){
                    print(time, "blocked on transit", "", barn, BqHead, numStalls);
                }else{
                    print(time, "", "", barn, BqHead, numStalls);
                }
                time++;
                barn[animalIndex] = animal;
                BQ newbq = (BQ)malloc(sizeof(struct bq));
                newbq->index = animalIndex;
                newbq->next = NULL;
                if(BqHead == NULL){
                    BqHead = newbq;
                    BqTail = BqHead;
                }else{
                    BqTail->next = newbq;
                    BqTail = newbq;
                }
                strncpy(animal->location, "barn", 255);
            }
            //no empty spots
            if(place == 0){
                print(time, "tend", animal->animal, barn, BqHead, numStalls);
                time++;
                if(Citer->next != NULL && Citer->next->time <= time){
                    print(time, "blocked on transit", "", barn, BqHead, numStalls);
                }else{
                    print(time, "", "", barn, BqHead, numStalls);
                }
                time++;
                BQ iter = BqHead;
                BQ last = iter;
                while(iter != NULL){
                    if(barn[iter->index]->scbit == 0){
                        removeAnimal = barn[iter->index];
                        barn[iter->index] = animal;
                        animal->scbit = 1;
                        BQ newElm = (BQ)malloc(sizeof(struct bq));
                        newElm->index = iter->index;
                        newElm->next = NULL;
                        BqTail->next = newElm;
                        BqTail = newElm;
                        strncpy(removeAnimal->location, "in transit", 255);
                        if(Citer->next != NULL && Citer->next->time <= time){
                            print(time, "blocked on transit", "", barn, BqHead, numStalls);
                        }else{
                            print(time, "", "", barn, BqHead, numStalls);
                        }
                        time++;
                        strncpy(removeAnimal->location, "nearby pasture", 255);
                        place = 3;
                        if(iter == BqHead){
                            BqHead = iter->next;
                            free(iter);
                        }else if(iter == BqTail){
                            BqTail = last;
                            free(iter);
                        }
                        break;
                    }else{
                        barn[iter->index]->scbit = 0;
                    }
                    last = iter;
                    iter = iter->next;
                }
                //everyone got second chance
                if(place != 3){
                    removeAnimal = barn[BqHead->index];
                    barn[BqHead->index] = animal;
                    animal->scbit = 1;
                    strncpy(animal->location, "barn", 255);
                    BQ newNew = (BQ)malloc(sizeof(struct bq));
                    newNew->index = BqHead->index;
                    newNew->next = NULL;
                    BqTail->next = newNew;
                    BqTail = newNew;
                    BQ old = BqHead;
                    BqHead = BqHead->next;
                    free(old);
                    strncpy(removeAnimal->location, "in transit", 255);
                    if(Citer->next != NULL && Citer->next->time <= time){
                        print(time, "blocked on transit", "", barn, BqHead, numStalls);
                    }else{
                        print(time, "", "", barn, BqHead, numStalls);
                    }
                    time++;
                    strncpy(removeAnimal->location, "nearby pasture", 255);
                }
            }
            Citer = Citer->next;
        }                       
    }
    print(time, "", "", barn, BqHead, numStalls);
}

Animal find(char *name){
    if(Ahead == NULL){
        Ahead = (Animal)malloc(sizeof(struct animal));
        strncpy(Ahead->animal, name, 255);
        Ahead->next = NULL;
        Atail = Ahead;
        return Ahead;
    }
    Animal iter = Ahead;
    while(iter != NULL){
        if(!strcmp(iter->animal, name)){
            return iter;
        }
        iter = iter->next;
    }
    iter = (Animal)malloc(sizeof(struct animal));
    strncpy(iter->animal, name, 255);
    iter->next = NULL;
    Atail->next = iter;
    Atail = iter;
    return iter;
}
        
void print(int time, char *action, char *animal, Animal barn[], BQ bqHead, int numStalls){
    printf("time: %d\n", time);
    if(strlen(animal) == 0 && strlen(action) != 0){
        printf("%s\n", action);
    }else if(strlen(action) != 0){
        printf("%s %s\n", action, animal);
    }
    if(bqHead == NULL){
        for(int i = 0; i < numStalls; i++){
            if(i == 0){
                printf("* barn 00: empty\n");
            }else{
                printf("  barn %02d: empty\n", i);
            }
        }
    }else{
        for(int i = 0; i < numStalls; i++){
            if(barn[i] == NULL){
                if(i == bqHead->index){
                    printf("* barn %02d: empty\n", i);
                }else{
                    printf("  barn %02d: empty\n", i);
                }
            }else{
                if(i == bqHead->index){
                    printf("* barn %02d: %s (bit=%d)\n", i, barn[i]->animal, barn[i]->scbit);
                }else{
                    printf("  barn %02d: %s (bit=%d)\n", i, barn[i]->animal, barn[i]->scbit);
                }
            }
        }
    }
    Animal iter = Ahead;
    int index = 0;
    while(iter != NULL){
        printf("  page %02d: %s @ %s\n", index, iter->animal, iter->location);
        iter = iter->next;
        index++;
    }
    printf("\n");
    return;
}

void removeB(Animal animal, BQ *BqHead, BQ *BqTail, Animal barn[], int numStalls){
    BQ Head = *BqHead;
    BQ Tail = *BqTail;
    int index = -1;
    for(int i = 0; i < numStalls; i++){
        if(barn[i] == animal){
            index = i;
            break;
        }
    }
    if(index == -1){
        return;
    }        
    barn[index] = NULL;
    BQ iter = Head;
    BQ last = iter;
    while(iter != NULL){
        if(iter->index == index){
            if(iter == Head){
                Head = iter->next;
                if(Head == NULL){
                    Tail = NULL;
                }
                free(iter);                
            }else if(iter == Tail){
                Tail = last;
                last->next = NULL;
                free(iter);
            }else{
                last->next = iter->next;
                free(iter);
            }
        }
        last = iter;
        iter = iter->next;
    }
    *BqTail = Tail;
    *BqHead = Head;
}

