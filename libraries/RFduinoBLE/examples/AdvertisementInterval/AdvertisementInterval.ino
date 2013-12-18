/*
The sketch demonstrates how to increase or decrease the Bluetooth Low Energy 4
Advertisement transmission interval.

Faster iterval = higher power consumption = lower connection latency
*/

#include <RFduinoBLE.h>

// pin 3 on the RGB shield is the green led
int led = 3;

// interval between advertisement transmissions ms (range is 20ms to 10.24s) - default 20ms
int interval = 675;  // 675 ms between advertisement transmissions

void setup() {
  // led used to indicate that the RFduino is advertising
  pinMode(led, OUTPUT);

  // this is the data we want to appear in the advertisement
  // (the deviceName length plus the advertisement length must be <= 18 bytes)
  // (a leading hypen causes the iphone to use instanteous rssi instead of n sample averaging)
  RFduinoBLE.advertisementData = "-data";
  
  // change the advertisement interval
  RFduinoBLE.advertisementInterval = interval;

  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
  // switch to lower power mode
  RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onAdvertisement(bool start)
{
  // turn the green led on if we start advertisement, and turn it
  // off if we stop advertisement
  
  if (start)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
}
