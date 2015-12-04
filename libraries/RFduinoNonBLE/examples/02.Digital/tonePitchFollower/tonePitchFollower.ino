/*
  Pitch follower
 
 Plays a pitch that changes based on a changing analog input
 
 circuit:
 * 8-ohm speaker through 100 Ohm resistor on digital pin 3
 * photoresistor on pin 5 to 3.3V
 * 4.7K resistor on pin 5 to ground
 
 created 21 Jan 2010
 modified 31 May 2012
 by Tom Igoe, with suggestion from Michael Flynn

This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/Tone2
 
 */


void setup() {
  // initialize serial communications (for debugging only):
  Serial.begin(9600);
}

void loop() {
  // read the sensor:
  int sensorReading = analogRead(5);
  // print the sensor reading so you know its range
  Serial.println(sensorReading);
  // map the analog input range (in this case, 400 - 1000 from the photoresistor)
  // to the output pitch range (120 - 1500Hz)
  // change the minimum and maximum input numbers below
  // depending on the range your sensor's giving:
  int thisPitch = map(sensorReading, 400, 1000, 120, 1500);

  // play the pitch:
  tone(3, thisPitch, 10);
  //delay(1);        // delay in between reads for stability
}






