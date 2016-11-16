
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
    void writeServ(int ang) {
      serv.write(750 + ((long)1000 * (long)ang) / (long)180);
    }
    void evaluateData() {
      //glatter ut data, fjerner sensorfeil og liknende
      for (int i = 0; i <= fieldOfView && false ; i++) {
        int tempVal = 0;
        tempVal += readings[(i + dataPoints - 1) % dataPoints]; //tar gjennomsnittet av tre målinger
        tempVal += readings[(i + dataPoints) % dataPoints];
        tempVal += readings[(i + dataPoints + 1) % dataPoints];
        readings[i] = tempVal / 3;                                //setter en avlesning lik gjennomsnittet av avlesningen og avlesningene over og under (avlesningen tatt på +/- 1 grad)
      }
      //finn lengste sammenhengende rekke med 1'ere (dvs lengste sammenhengende utslag=mest sannsynlig at er et "stort" objekt slik som en zumo)
      int currentStart = 0;
      int bestStart = -1, bestLength = 0;
      for (int i = 0; i < dataPoints; i++) {
        if (!(readings[i] > sensorThreshold)) {
          if (i - currentStart > bestLength) {
            bestStart = currentStart;
            bestLength = i - currentStart;
          }
          currentStart = i + 1;
        }
      }
      if (dataPoints - 1 - currentStart > bestLength) {
        bestStart = currentStart;
        bestLength = dataPoints - 1 - currentStart;
      }
      //evaluerer datane fra forrige ledd og ser om det kan være en fiende, og hvilken vinkel det i så fall korresponderer til
      if (bestLength > 3) {
        enemyInView = true;
        enemyAngle += leftmostAngle + bestStart + bestLength / 2; //objektets posisjon blir starten av "siluetten" pluss halve lengden, altså midten av siluetten, + hvileposisjonen (data[0] er tatt fra vinkel gitt av leftmostAngle)
        enemyAngle /= 2;
        enemyWidth = bestLength >> 1;
      } else {
        enemyInView = false;                                     //hvis ingen siluetter har stor nok størrelse så er det antakelig ingen fiende i sikte
      }
    }

  public:
    void calibrate() {
      writeServ(leftmostAngle);
      delay(250);                                           //venter en stund for å være sikker på at servo rekker å nå hvileposisjon
      for (int i = 0; i <= fieldOfView; i++) {
        calibrationReadings[i] = 0;
        for (int j = 0; j < 100; j++) {                       //gjør fem målinger med litt tid imellom
          calibrationReadings[i] += analogRead(sensorPin);  //summerer opp de fem målingene
          delayMicroseconds(25);
        }
        calibrationReadings[i] = calibrationReadings[i] / 100;                       //deler på antallet målinger
        writeServ(leftmostAngle + i);                      //skriver neste vinkel
        delayMicroseconds(500);
      }
      writeServ(leftmostAngle);                            //går tilbake til hvileposisjon
      delay(250);
    }

    cameraHandler() {
      currentAngle = 0;
      readings = new int[dataPoints];
      calibrationReadings = new int[dataPoints];
    }

    void attach(unsigned char _servoPin, unsigned char _sensorPin) {
      servoPin = _servoPin;
      sensorPin = _sensorPin;
      serv.attach(_servoPin);
      writeServ(90);
    }

    void tic() {
      readings[currentAngle] = analogRead(sensorPin) - calibrationReadings[currentAngle]; //leser av sensoren og trekker fra kalibreringsverdien
      if (goingRight) {
        currentAngle++;
      } else {
        currentAngle--;
      }
      if (currentAngle % 10 == 0) {
        evaluateData();
      }
      if (currentAngle > fieldOfView || currentAngle < 0) {
        currentAngle = constrain(currentAngle, 0, fieldOfView);
        goingRight = !goingRight;
      } else {
        writeServ(leftmostAngle + currentAngle);
      }
    }

};
