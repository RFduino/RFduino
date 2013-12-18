/*
 Input Pullup Serial
 
 This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a 
 digital input on pin 5 and prints the results to the serial monitor.
 
 The circuit: 
 * Momentary switch attached from pin 5 to ground 
 * LED on pin 3
 
 Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal 
 14K-ohm resistor is pulled to 3.3V. This configuration causes the input to 
 read HIGH when the switch is open, and LOW when it is closed. 
 
 created 14 March 2012
 by Scott Fitzgerald
 
 http://www.arduino.cc/en/Tutorial/InputPullupSerial
 
 This example code is in the public domain
 
 */

void setup(){
  //start serial connection
  Serial.begin(9600);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(5, INPUT_PULLUP);
  pinMode(3, OUTPUT); 

}

void loop(){
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(5);
  //print out the value of the pushbutton
  Serial.println(sensorVal);
  
  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on pin 3 when the 
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH) {
    digitalWrite(3, LOW);
  } 
  else {
    digitalWrite(3, HIGH);
  }
}



