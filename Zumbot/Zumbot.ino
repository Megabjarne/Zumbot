#include "STL.h"
#include "FuncQueHandler.cpp"
#include "CamHandler.cpp"
//#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
//#include <ZumoReflectanceSensorArray.h>
//#include <Servo.h>
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
  factory.make_thread(periodic, 1, &visionHandler);
  factory.make_thread(periodic, 50, &debugging);
  factory.make_thread(periodic, 50, &borderCheck);
}

void loop() {
  factory.tic();
}

void visionHandler() {
  Serial.println("I see!");
  vision.tic();
}

void debugging() {
  if (panicTime < millis()) {
    if (vision.enemyInView ) {
      //Serial.print("FOUND: "); Serial.println(vision.enemyAngle);
      if (vision.enemyAngle - vision.enemyWidth > 110) {
        motors.setSpeeds(200, 100 );
      } else {
        if (vision.enemyAngle + vision.enemyWidth < 70) {
          motors.setSpeeds(100, 200);
        } else {
          motors.setSpeeds(400, 400);
        }
      }
    } else {
      motors.setSpeeds(-100, 100);
      //Serial.println("Nothing!");
    }
  } else {
    motors.setSpeeds(-400, -400);
  }
}

void borderCheck() {
  unsigned char pins [] = {4, 5};
  QTRSensorsRC qtrs(pins, 2, 500, QTR_NO_EMITTER_PIN);
  unsigned int sensorArr[2];
  Serial.print("start");Serial.flush();
  qtrs.read(sensorArr);
  Serial.print("end");Serial.flush();
  if (sensorArr[0] < 500 || sensorArr[1] < 500) {
    motors.setSpeeds(-400, -400);
    panicTime = millis()+250;
  }
  Serial.print(sensorArr[0]);Serial.print("  ");Serial.println(sensorArr[1]);Serial.flush();
}
