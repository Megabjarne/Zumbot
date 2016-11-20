#include "FuncQueHandler.h"

FuncQueHandler factory;

void loop(){
  factory.tic();
}

int FuncQueHandler::gen_key() {
  int ID = 0;
  for (int i = 0; i < queueSize; i++) {
    if (queue[i].processID >= ID && queue[i].processID != -1)
    {
      ID = queue[i].processID + 1;
    }
  }
  return ID;
}

FuncQueHandler::FuncQueHandler() {
  for (int i = 0; i < queueSize; i++) {
    queue[i].processID = -1;
  }
}

void FuncQueHandler::add(callRequest _call) {
  for (int i = 0; i < queueSize; i++) {
    if (queue[i].processID == -1) {
      queue[i] = _call;
      return;
    }
  }
}

int FuncQueHandler::make_thread(int _flag, int _time, funcptr _function) {
  callRequest newcall;
  newcall.processID = gen_key();
  newcall.flag = _flag;
  newcall.initTime = millis();
  newcall.Time = _time;
  newcall.function = _function;
  add(newcall);
  return newcall.processID;
}

void FuncQueHandler::tic() {
  int bestCandidate = -1;
  unsigned long longestOverdue = 0;
  unsigned long longestIdle = 0;
  for (int i = 0; i < queueSize; i++) {
    if (queue[i].processID != -1) { //if in use
      switch (queue[i].flag) {
        case idle:
          if (longestOverdue == 0) {
            if (millis() - (queue[i].initTime) > longestIdle) { //if idle for longer than others
              longestIdle = millis() - queue[i].initTime;
              bestCandidate = i;
            }
          }
          break;
        case timed:
        case periodic:
        case strictPeriodic: //hvis en av disse tre tidsbaserte typene
          if (millis() > (queue[i].initTime + queue[i].Time)) { //if overdue
            if (millis() - (queue[i].initTime + queue[i].Time) > longestOverdue) { //if most overdue
              longestOverdue = millis() - (queue[i].initTime + queue[i].Time);
              bestCandidate = i;
            }
          }
          break;
      }
    }
  }
  if (bestCandidate != -1) {
    if (DEBUG == true) {
      DEBUGtime = millis();
    }
    (*queue[bestCandidate].function)(); //calls functionpointer
    if (DEBUG == true) {
      Serial.print("Prosess \"");  //Display time thread execution used
      Serial.print(queue[bestCandidate].processID);
      Serial.print("\" used ");
      Serial.print(millis() - DEBUGtime);
      Serial.println("ms");
    }
    switch (queue[bestCandidate].flag) {
      case idle:
        queue[bestCandidate].initTime = millis(); //reset initTime, but keep thread as idlethread
        break;
      case timed:
        queue[bestCandidate].processID = -1; //kills thread
        break;
      case strictPeriodic:
        queue[bestCandidate].initTime += queue[bestCandidate].Time; //if due every 1000ms, just add 1000ms between "startups"
        break;
      case periodic:
        queue[bestCandidate].initTime = millis(); //if ordinary periodic, just use time at end of execution, that's here, now
        break;
    }
  }
}







