/*
The sketch demonstrates iBecaon from an RFduino
*/

#include <RFduinoBLE.h>

// pin 3 on the RGB shield is the green led
int led = 3;

void setup() {
  // led used to indicate that iBeacon has started
  pinMode(led, OUTPUT);

  // do iBeacon advertising
  RFduinoBLE.iBeacon = true;
  
  // override the default iBeacon settings
  uint8_t uuid[16] = {0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0};
  memcpy(RFduinoBLE.iBeaconUUID, uuid, sizeof(RFduinoBLE.iBeaconUUID));
  RFduinoBLE.iBeaconMajor = 1234;
  RFduinoBLE.iBeaconMinor = 5678;
  
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
