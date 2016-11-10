#include <Servo.h>
#include <arduino.h>
class cameraHandler
{
  private:
    const bool DEBUG = true;
    const int fieldOfView = 90;
    const int leftmostAngle = (180 - fieldOfView) >> 1;
    const int dataPoints = fieldOfView + 1;
    const int sensorThreshold = 150;
    unsigned char servoPin;
    unsigned char sensorPin;
    int currentAngle = 0;
    int* readings;
    int* calibrationReadings;
    Servo serv;
    bool goingRight = true;

  public:
    boolean enemyInView = false;
    int enemyAngle = 0;

  private:
    void evaluateData() {
      //glatter ut data, fjerner sensorfeil og liknende
      for (int i = 0; i <= fieldOfView && false ; i++) {
        int tempVal = 0;
        tempVal += readings[(i + dataPoints - 1) % dataPoints]; //tar gjennomsnittet av tre målinger
        tempVal += readings[(i + dataPoints) % dataPoints];
        tempVal += readings[(i + dataPoints + 1) % dataPoints];
        readings[i] = tempVal / 3;                                //setter en avlesning lik gjennomsnittet av avlesningen og avlesningene over og under (avlesningen tatt på +/- 1 grad)
      }
      //skiller mellom avlesninger over og under en threshold
      for (int i = 0; i < dataPoints; i++) {
        readings[i] = (readings[i] > sensorThreshold);          //gjør datapunkt om til 0 hvis ikke over threshold og 1 hvis over threshold
      }
      for (int i = 0; i < dataPoints; i++) {
        Serial.print(readings[i]);
      } Serial.println();
      //finn lengste sammenhengende rekke med 1'ere (dvs lengste sammenhengende utslag=mest sannsynlig at er et "stort" objekt slik som en zumo)
      int currentStart = 0;
      int bestStart = -1, bestLength = 0;
      for (int i = 0; i < dataPoints; i++) {
        if (!readings[i]) {
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
      if (bestLength > 4) {
        enemyInView = true;
        enemyAngle += leftmostAngle + bestStart + bestLength / 2; //objektets posisjon blir starten av "siluetten" pluss halve lengden, altså midten av siluetten, + hvileposisjonen (data[0] er tatt fra vinkel gitt av leftmostAngle)
      enemyAngle/=2;
      } else {
        enemyInView = false;                                     //hvis ingen siluetter har stor nok størrelse så er det antakelig ingen fiende i sikte
      }
    }

  public:
    void calibrate() {
      serv.write(leftmostAngle);
      delay(250);                                           //venter en stund for å være sikker på at servo rekker å nå hvileposisjon
      for (int i = 0; i <= fieldOfView; i++) {
        calibrationReadings[i] = 0;
        for (int j = 0; j < 100; j++) {                       //gjør fem målinger med litt tid imellom
          calibrationReadings[i] += analogRead(sensorPin);  //summerer opp de fem målingene
          delayMicroseconds(100);
        }
        calibrationReadings[i] = calibrationReadings[i] / 100;                       //deler på antallet målinger
        serv.write(leftmostAngle + i);                      //skriver neste vinkel
        delay(3);
      }
      serv.write(leftmostAngle);                            //går tilbake til hvileposisjon
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
      serv.write(90); delay(1000); serv.write(45); delay(1000);
    }

    void tic() {
      readings[currentAngle] = analogRead(sensorPin) - calibrationReadings[currentAngle]; //leser av sensoren og trekker fra kalibreringsverdien
      if (goingRight) {
        currentAngle++;
      }else{
        currentAngle--;
      }
      if (currentAngle > fieldOfView || currentAngle<0) {
        evaluateData();
        currentAngle=constrain(currentAngle,0,fieldOfView);
        goingRight=!goingRight;
      } else {
        serv.write(leftmostAngle + currentAngle);

      }
    }

};
