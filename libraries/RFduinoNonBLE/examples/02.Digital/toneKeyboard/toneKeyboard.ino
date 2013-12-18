/*
  keyboard
 
 Plays a pitch that changes based on a changing analog input
 
 circuit:
 * 3 force-sensing resistors from +3.3V to pin 2 through 4
 * 3 10K resistors from pin 2 through 4 to ground
 * 8-ohm speaker on pin 5 through 100 Ohm resistor
 
 created 21 Jan 2010
 modified 9 Apr 2012
 by Tom Igoe 

This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/Tone3
 
 */

#include "pitches.h"

const int threshold = 10;    // minimum reading of the sensors that generates a note

// notes to play, corresponding to the 3 sensors:
int notes[] = {0, 0,
  NOTE_A4, NOTE_B4, NOTE_C3 };

void setup() {

}

void loop() {
  for (int thisSensor = 2; thisSensor < 5; thisSensor++) {
    // get a sensor reading:
    int sensorReading = analogRead(thisSensor);

    // if the sensor is pressed hard enough:
    if (sensorReading > threshold) {
      // play the note corresponding to this sensor:
      tone(5, notes[thisSensor], 20);
    } 
  }
}
