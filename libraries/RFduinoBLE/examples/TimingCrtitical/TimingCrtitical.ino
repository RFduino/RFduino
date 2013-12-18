/*
The sketch demonstrates how to integrate timing critical code
with Bluetooth Low Energy 4.
*/

#include <RFduinoBLE.h>

// pin 3 on the RGB shield is the green led
int led = 3;

// flag used to toggle the led
bool flag = false;

void setup() {
  // led used to indicate that the RFduino is advertising
  pinMode(led, OUTPUT);

  // this is the data we want to appear in the advertisement
  // (the deviceName length plus the advertisement length must be <= 18 bytes
  RFduinoBLE.advertisementData = "data";
  
  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
  // pause while the radio is active
  while (RFduinoBLE.radioActive)
    ;
    
  // timing critial code that must not be interrupted by radio events
  flag = ! flag;
  if (flag)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  // an artifical delay to simulate something more realistic
  delay(1);
}

