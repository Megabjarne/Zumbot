#ifndef _bordercheckhandler_h
#define _bordercheckhandler_h
#include <arduino.h>

class borderCheckHandler
{
  private:
    unsigned char leftSensor, rightSensor;
    const int threshold = 500;
    const bool blackIsSafe = true;
  public:
    borderCheckHandler(unsigned char _l, unsigned char _r);
    bool left();
    bool right();
};

#endif

