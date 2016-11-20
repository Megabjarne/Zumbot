#ifndef _funcquehandler_h
#define _funcquehandler_h
#include <arduino.h>
typedef void (*funcptr)();
const int queueSize = 10; //Det maksimale antallet tråder som kan ligge i køen

enum flags {
  idle,
  timed,
  periodic,
  strictPeriodic
};

class FuncQueHandler
{
  private:
    struct callRequest {
      int processID;
      int flag;
      long unsigned int initTime;
      unsigned int Time;
      funcptr function;
    };
    callRequest queue[queueSize];
    int gen_key();
  public:
    bool DEBUG = false;
    unsigned long DEBUGtime;
    FuncQueHandler();
    void add(callRequest _call);
    int make_thread(int _flag, int _time, funcptr _function);
    void tic();
};

extern FuncQueHandler factory;
#endif

