/*
  DigitalReadSerial
 Reads a digital input on pin 5, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */

// digital pin 5 has a pushbutton #1 in the RGB LED shield attached to it. Give it a name:
int pushButton_1 = 5;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton_1, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton_1);
  // print out the state of the button:
  Serial.println(buttonState);
  delay(10);        // delay in between reads for stability
}



