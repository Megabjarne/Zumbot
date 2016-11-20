#include "STL.h"
#include "FuncQueHandler.cpp"
#include "CamHandler.cpp"
//#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
ZumoMotors motors;
//ZumoBuzzer buzzer;
FuncQueHandler factory;
cameraHandler vision;
ServoTimer2 serv;
unsigned long panicTime = 0;


void setup() {
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  Serial.begin(9600);
  Pushbutton butt(ZUMO_BUTTON);
  butt.waitForButton();
  vision.attach(2, 0);
  butt.waitForButton();
  Serial.println("calibrate");
  vision.calibrate();
  Serial.println("c2");
  butt.waitForButton();
  pinMode(3, OUTPUT);
  pinMode(13,OUTPUT);
  factory.make_thread(periodic, 1, &visionHandler);
  factory.make_thread(periodic, 50, &debugging);
  factory.make_thread(periodic, 3, &borderCheck);
}

void loop() {
  factory.tic();
}

void visionHandler() {
  vision.tic();
}

void debugging() {
  digitalWrite(13,vision.enemyInView);
  if (panicTime < millis()) {
    if (vision.enemyInView ) {
      if (vision.enemyAngle - vision.enemyWidth > 110) {
        motors.setSpeeds(200, -100 );
      } else if (vision.enemyAngle + vision.enemyWidth < 70) {
        motors.setSpeeds(-100, 200);
      } else {
        motors.setSpeeds(400, 400);
      }
    } else {
      motors.setSpeeds(400, -100);
    }
  } else {
    motors.setSpeeds(-400, -400);
  }
}
void borderCheck() {
  if (analogRead(1) > 500 || analogRead(2) > 500) {
    panicTime = millis() + 500;
  }
}
