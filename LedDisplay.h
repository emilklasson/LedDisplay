/*
 * LedDisplay.h
 * Print integers to a 4 digits 7 segments display
 * using ::Timer1::
 */

#ifndef LedDisplay_h
#define LedDisplay_h

#include "Arduino.h"
#include <TimerOne.h>

class LedDisplay {
  public:
    LedDisplay(int latchPin, int clockPin, int dataPin);
    void init();
    void write(int number);
  private:
    void blinkAll_2Bytes(int n, int d);
    static void writeToDisplay();
    static int _latchPin;
    static int _clockPin;
    static int _dataPin;
    static int _nextDigitToWrite;
};

#endif
