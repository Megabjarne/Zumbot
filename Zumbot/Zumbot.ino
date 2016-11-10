#include "STL.h"
#include "FuncQueHandler.cpp"
#include "CamHandler.cpp"
#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
//#include <ZumoReflectanceSensorArray.h>
//#include <Servo.h>

//ZumoMotors motors;
//ZumoBuzzer buzzer;
FuncQueHandler factory;
cameraHandler vision;

void setup() {
  //Servo serv;serv.attach(5);serv.write(90);
  Pushbutton butt(ZUMO_BUTTON);
  Serial.begin(9600);
  butt.waitForButton();
   vision.attach(5,0);
  //butt.waitForButton();
  Serial.println("calibrate");
  vision.calibrate();
  Serial.println("c2");
  //butt.waitForButton();
  pinMode(3,OUTPUT);
  factory.make_thread(periodic,5,&visionHandler);
  factory.make_thread(periodic,2000,&debugging);
}

void loop() {
  factory.tic();
}

void visionHandler(){
  vision.tic();
}

void debugging(){
  if (vision.enemyInView){
    Serial.print("FOUND: ");Serial.println(vision.enemyAngle);
  }else{
    Serial.println("Nothing!");
  }
}

