#include "setup.h"
#include <stdio.h>

int calcTime(int curFloor, Rider theRider);

int chooseNext(int floors, Rider waiting[], int curFloor, Rider elevator){
  int lowTime = 0;
  int ret = 0;
  int time = 0;
  
  if(elevator != NULL){
    return elevator->to-1;
  }
  
  for(int i = 0; i < floors; i++){
    if(waiting[i] != NULL){
      time = calcTime(curFloor, waiting[i]);
      if(lowTime == 0){
        lowTime = time;
        ret = i;
      }else if(time < lowTime){
        lowTime = time;
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

int calcTime(int curFloor, Rider theRider){
  int ret = curFloor - (theRider->from) -1;
  ret = (ret < 0)?(-ret):(ret);
  int dist = theRider->from - theRider->to;
  dist = (dist < 0)?(-dist):(dist);

  return ret+dist;
}
