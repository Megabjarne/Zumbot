#include "FuncQueHandler.h"
#include "cameraHandler.h"
#include "borderCheckHandler.h"
#include <ZumoMotors.h>
#include <Pushbutton.h>

const unsigned char digital_visionServo = 2;        //which pin the cameraservo is connected to
const unsigned char analog_visionSensor = 0;        //which pin the "camera" sensor is connected to
const unsigned char analog_borderLeft = 1;          //which pin the leftmost bordersensor is connected to
const unsigned char analog_borderRight = 2;         //which pin the rightmost bordersensor is connected to
const unsigned char digital_hasTargetLED = 13;      //which pin the LED to indicate that an target is aquired is connected to

const unsigned int panicDuration = 500;             //for how long the bot should panic after detecting an edge
const unsigned char acceptableRammingDeviation = 10; //how large an deviation is acceptable for ramming

ZumoMotors motors;
cameraHandler vision;
borderCheckHandler border(analog_borderLeft, analog_borderRight);
Pushbutton butt(ZUMO_BUTTON);

unsigned long panicTime = 0;

void setup() {
  Serial.begin(9600);
  butt.waitForButton();
  pinMode(digital_hasTargetLED, OUTPUT);
  configureVision();
  butt.waitForButton();
  //play music for 5 seconds
  factory.make_thread(periodic, 1, &visionTic);       //factory is a functionqueuer found in the "FuncQueHandler"-files, adds the visionTic-function to be executed periodically every 1 milliseconds
  factory.make_thread(periodic, 50, &debugging);
}

void configureVision() {
  vision.attach(digital_visionServo, analog_visionSensor); //attaches servo and stores what analogpin the sensor is connected to
  vision.calibrate();  //performes a sweep to calibrate the sensor
}

void visionTic() {
  vision.tic(); //cannot reference memberfunctions, which is why this local function executing the memberfunction is here
}

void debugging() {
  digitalWrite(digital_hasTargetLED, vision.enemyInView);
  if (border.left() || border.right()) {
    panicTime = millis() + panicDuration;
  }
  if (panicTime < millis()) {
    if (!vision.enemyInView ) {
      motors.setSpeeds(400, -100);
    } else {
      if (vision.enemyAngle - vision.enemyWidth > 90 + acceptableRammingDeviation) {
        motors.setSpeeds(200, -100 );
      } else if (vision.enemyAngle + vision.enemyWidth < 90 - acceptableRammingDeviation) {
        motors.setSpeeds(-100, 200);
      } else {
        motors.setSpeeds(400, 400);
      }
    }
  } else {
    motors.setSpeeds(-400, -400);
  }
}
