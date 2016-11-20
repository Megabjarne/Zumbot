#ifndef _camerahandler_h
#define _camerahandler_h
#include "ServoTimer2.h"
#include <arduino.h>

class cameraHandler
{
  private:
    const bool DEBUG = false;
    const int fieldOfView = 135;
    const int leftmostAngle = (180 - fieldOfView) >> 1;
    const int dataPoints = fieldOfView + 1;
    const int sensorThreshold = 75;
    unsigned char servoPin;
    unsigned char sensorPin;
    int currentAngle = 0;
    int* readings;
    int* calibrationReadings;
    ServoTimer2 serv;
    bool goingRight = true;

  public:
    boolean enemyInView = false;
    int enemyAngle = 0;
    int enemyWidth = 0;

  private:
    void writeServ(int ang);
    void evaluateData();

  public:
    void calibrate();
    cameraHandler();
    void attach(unsigned char _servoPin, unsigned char _sensorPin);
    void tic();
};
#endif
