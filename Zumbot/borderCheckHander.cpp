#include "borderCheckHandler.h"

borderCheckHandler::borderCheckHandler(unsigned char _l, unsigned char _r): leftSensor(_l), rightSensor(_r) {}

bool borderCheckHandler::left() {
  if (blackIsSafe) {
    return blackIsSafe ? analogRead(leftSensor) > threshold : analogRead(leftSensor) < threshold;
  }
}

bool borderCheckHandler::right() {
  if (blackIsSafe) {
    return blackIsSafe ? analogRead(rightSensor) > threshold : analogRead(rightSensor) < threshold;
  }
}

