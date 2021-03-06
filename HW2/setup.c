#include <stdio.h>
#include <stdlib.h>
#include "setup.h"

int main(int argc, char *argv[]){
  if(argc < 1){
    printf("Usage: %s RIDER_FILE\n", argv[0]);
    return 1;
  }
  FILE *fp = fopen(argv[1], "r");
  if(!fp){
    printf("Could not open file \n");
    return 1;
  }
  
  int floors = 0;
  int numRiders = 1;
  int check = 0;
  Rider first = (Rider)malloc(sizeof(rid));
  Rider last = first;
  Rider next = NULL;
  
  fscanf(fp, "%d", &floors);
  fscanf(fp, "%d %s %d %d", &first->time, first->id, &first->from, &first->to);
  first->next = NULL;
  while(!feof(fp)){
    next = (Rider)malloc(sizeof(rid));
    check = fscanf(fp, "%d %s %d %d", &next->time, next->id, &next->from, &next->to);
    if(check < 3){
      break;
    }
    last->next = next;
    next->next = NULL;
    last = next;
    numRiders++;
  }
  runElevator(floors, first, numRiders);
}

void printState(int time, int curFloor, int targetFloor, int numFloors, Rider waiting[], Rider elevator){
  Rider print = NULL;
  printf("time: %d\n", time);
  for(int i = 0; i < numFloors; i++){
    if(i == targetFloor){
      printf("* ");
    }else{
      printf("  ");
    }
    printf("Floor %d: ", i+1);
    if(i == curFloor){
      if(elevator == NULL){
        printf("elevator (); ");
      }else{
        printf("elevator (%s); ", elevator->id);
      }
    }
    if(waiting[i] != NULL){
      print = waiting[i];
      while(print != NULL){
        printf("%s @ t=%d, %d;", print->id, print->time, print->to);
        print = print->next;
      }
    }
    printf("\n");
  }
  printf("\n");
}
  
void runElevator(int floors, Rider first, int numRiders){
  int numWaiting = 0;
  int ffrom = 0;
  int curFloor = 0;
  int toFloor = 0;
  int inUse = 0;
  int waitTime = 0;
  Rider elevator = NULL;
  Rider current = first;
  Rider waiting[floors];
  Rider iter = NULL;
  int time = 1;
  for(int i = 0; i < floors; i++){
    waiting[i] = NULL;
  }
  
  while(1){
    //metrics
    for(int i = 0; i < floors; i++){
      if(waiting[i] != NULL){
        waitTime++;
      }
    }
    
    if(elevator != NULL){
      inUse++;
      waitTime++;
    }
  
    while((current != NULL) && (current->time == time)){
      ffrom = current->from-1;
      if(waiting[ffrom] == NULL){
        waiting[ffrom] = current;
        current = current->next;
        waiting[ffrom]->next = NULL;
      }else{
        iter = waiting[ffrom];
        while(iter->next != NULL){
          iter = iter->next;
        }
        iter->next = current;
        current = current->next;
        iter->next->next = NULL;
      }
    }
    
    if(curFloor == toFloor){
      if(!elevator){
        if(waiting[curFloor] != NULL){
          elevator = waiting[curFloor];
          waiting[curFloor] = waiting[curFloor]->next;
          toFloor = elevator->to-1;
        }else{
          toFloor = chooseNext(floors, waiting, curFloor, elevator);
        }
      }else{
        elevator = NULL;
        toFloor = chooseNext(floors, waiting, curFloor, elevator);
        if(toFloor == curFloor){
          elevator = waiting[curFloor];
          if(waiting[curFloor] != NULL){
            waiting[curFloor] = waiting[curFloor]->next;
          }
          toFloor = chooseNext(floors, waiting, curFloor, elevator);
        }
      }
    }
    
    printState(time, curFloor, toFloor, floors, waiting, elevator);
    
    if(curFloor < toFloor){
      curFloor++;
    }else if(curFloor > toFloor){
      curFloor--;
    }
    
    
    if(current == NULL && elevator == NULL){
      numWaiting = 0;
      for(int i = 0; i < floors; i++){
        if(waiting[i] != NULL){
          numWaiting++;
        }
      }
      if(numWaiting == 0){
        printf("Average wait time: %.2f\n", (float)waitTime/(float)numRiders);
        printf("Average utilization: %2d%%\n", (100*inUse)/time);
        break;
      }
    }
    time++;
  }
}
    
  
  
