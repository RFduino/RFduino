/*
  Multiple tone player
 
 Plays multiple tones on multiple pins in sequence
 
 circuit:
 * 3 8-ohm speaker on digital pins 2, 3, and 4
 
 created 8 March 2010
 by Tom Igoe 
 based on a snippet from Greg Borenstein

This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/Tone4
 
 */

void setup() {

}

void loop() {
   // turn off tone function for pin 4:
   noTone(4);			
  // play a note on pin 2 for 200 ms:
  tone(2, 440, 200);
  delay(200);

  // turn off tone function for pin 2:
  noTone(2);
  // play a note on pin 3 for 500 ms:
  tone(3, 494, 500);
  delay(500);
  
  // turn off tone function for pin 3:
  noTone(3);  
  // play a note on pin 4 for 500 ms:
  tone(4, 523, 300);
  delay(300);

}
