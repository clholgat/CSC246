#include "setup.h"
#include <stdio.h>

int chooseNext(int floors, Rider waiting[], int curFloor, Rider elevator){
  int lowTime = 0;
  int ret = 0;
  
  if(elevator != NULL){
    return elevator->to-1;
  }
  
  for(int i = 0; i < floors; i++){
    if(waiting[i] != NULL){
      if(lowTime == 0){
        lowTime = waiting[i]->time;
        ret = i;
      }else if(waiting[i]->time < lowTime){
        lowTime = waiting[i]->time;
        ret = i;
      }
    }
  }
  
  if(lowTime == 0){
    return curFloor;
  }else{
    return ret;
  }
}
