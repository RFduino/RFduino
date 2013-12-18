/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 3, when pressing a pushbutton attached to pin 5. 
 
 
 The circuit:
 * LED attached from pin 3 to ground 
 * pushbutton attached to pin 5 from +5V
 * 10K resistor attached to pin 5 from ground
 
 * Note: on the RGB LED shield there is already a green LED on the board
 attached to pin 3 and Button 1 attached to pin 5
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin1 = 5;     // the number of the pushbutton pin
const int ledPinGreen = 3;    // the number of the green LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPinGreen, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);     
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin1);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPinGreen, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledPinGreen, LOW); 
  }
}
