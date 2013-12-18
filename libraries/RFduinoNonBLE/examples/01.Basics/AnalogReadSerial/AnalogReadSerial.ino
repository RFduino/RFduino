/*
  AnalogReadSerial
  Reads an analog input on pin 5, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin 5, and the outside pins to +3.3V and ground.
 
 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 5:
  int sensorValue = analogRead(5);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(10);        // delay in between reads for stability
}
