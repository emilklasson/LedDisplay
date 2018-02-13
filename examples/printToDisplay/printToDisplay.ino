#include <LedDisplay.h>

// create an instance of LedDisplay and specify pins to use
// LedDisplay::LedDisplay(int latchPin, int clockPin, int dataPin)
// LatchPin connected to ST_CP of 74HC595
// ClockPin connected to SH_CP of 74HC595
// DataPin connected to DS of 74HC595

LedDisplay disp(8, 12, 11);

void setup() {
  Serial.begin(9600);

  Serial.write("Printing numbers to leds");
  disp.init();
}

void loop() {
  for (int digit = 0; digit < 1000; digit++) {
    disp.write(digit);
    delay(500);
  }
}
