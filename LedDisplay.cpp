#include "Arduino.h"
#include "LedDisplay.h"
#include <TimerOne.h>

static byte dataArrayNUMBER[10];
static byte dataArrayDIGIT[4];
static byte dataDisplayNumber[4];

static int LedDisplay::_latchPin;
static int LedDisplay::_clockPin;
static int LedDisplay::_dataPin;

static int LedDisplay::_nextDigitToWrite = 0;

LedDisplay::LedDisplay(int latchPin, int clockPin, int dataPin) {
  //Pin connected to ST_CP of 74HC595
  _latchPin = latchPin;
  //Pin connected to SH_CP of 74HC595
  _clockPin = clockPin;
  //Pin connected to DS of 74HC595
  _dataPin = dataPin;
  
  
  pinMode(_latchPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);

  //Arduino doesn't seem to have a way to write binary straight into the code 
  //so these values are in HEX.  Decimal would have been fine, too. 
  dataArrayNUMBER[0] = 0x3F; //0
  dataArrayNUMBER[1] = 0x06; //1
  dataArrayNUMBER[2] = 0x5B; // 2
  dataArrayNUMBER[3] = 0x4F; // 3
  dataArrayNUMBER[4] = 0x66; // 4
  dataArrayNUMBER[5] = 0x6D; // 5
  dataArrayNUMBER[6] = 0x7D; // 6
  dataArrayNUMBER[7] = 0x07; // 7
  dataArrayNUMBER[8] = 0x7F; //8
  dataArrayNUMBER[9] = 0x6F; // 9


  dataArrayDIGIT[0] = 0x1; //1
  dataArrayDIGIT[1] = 0x2; //2
  dataArrayDIGIT[2] = 0x4; //3
  dataArrayDIGIT[3] = 0x8; //4

  // Clear all display numbers
  dataDisplayNumber[0] = 0x0;
  dataDisplayNumber[1] = 0x0;
  dataDisplayNumber[2] = 0x0;
  dataDisplayNumber[3] = 0x0;
}

void LedDisplay::init() {
  LedDisplay::blinkAll_2Bytes(2,200);

  // Starting the interrupt timer that will call our writeToDisplay method
  // to display current number at current position (_nextDigitToWrite)
  Timer1.initialize(5000);
  Timer1.attachInterrupt(LedDisplay::writeToDisplay);
}

// number you want to display
void LedDisplay::write(int number) {
  int ones = (number%10);
  int tens = ((number/10)%10);
  int hundreds = ((number/100)%10);
  int thousands = (number/1000);

  dataDisplayNumber[0] = number >= 1000 ? dataArrayNUMBER[thousands] : 0x0;
  dataDisplayNumber[1] = number >= 100 ? dataArrayNUMBER[hundreds] : 0x0;
  dataDisplayNumber[2] = number >= 10 ? dataArrayNUMBER[tens] : 0x0;
  dataDisplayNumber[3] = dataArrayNUMBER[ones];
}

// Interrupt function thas shiftsout current digit
void LedDisplay::writeToDisplay() {
  digitalWrite(_latchPin, LOW);
  shiftOut(_dataPin, _clockPin, MSBFIRST, dataArrayDIGIT[_nextDigitToWrite]);
  shiftOut(_dataPin, _clockPin, MSBFIRST, dataDisplayNumber[_nextDigitToWrite]);
  digitalWrite(_latchPin, HIGH);
  
  _nextDigitToWrite = (_nextDigitToWrite + 1) % 4;
}

// blinks the whole register based on the number of times you want to 
// blink "n" and the pause between them "d"
// starts with a moment of darkness to make sure the first blink
// has its full visual effect.
void LedDisplay::blinkAll_2Bytes(int n, int d) {  
  digitalWrite(_latchPin, LOW);
  shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
  shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
  digitalWrite(_latchPin, HIGH);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(_latchPin, LOW);
    shiftOut(_dataPin, _clockPin, MSBFIRST, 255);
    shiftOut(_dataPin, _clockPin, MSBFIRST, 255);
    digitalWrite(_latchPin, HIGH);
    delay(d);
    digitalWrite(_latchPin, LOW);
    shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
    shiftOut(_dataPin, _clockPin, MSBFIRST, 0);
    digitalWrite(_latchPin, HIGH);
    delay(d);
  }
}
