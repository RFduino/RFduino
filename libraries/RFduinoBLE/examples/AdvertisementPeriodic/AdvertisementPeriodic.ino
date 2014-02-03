/*
The sketch demonstrates how to do power conservative Bluetooth Low Energy 4
Advertisement with the RFduino.  Advertisement of "111" is done for an
interval in seconds, then advertisement of "222" is done for the same
interval, then the RFduino is switched ultra low power mode.  The RFduino
can be woken up by pressing Button 1, at which time the device will
advertise again.
*/

#include <RFduinoBLE.h>

// pin 3 on the RGB shield is the green led
int led = 3;

// pin 5 on the RGB shield is button 1
int button = 5;

// time to advertise in milliseconds
int duration = SECONDS(5);

void advertise(const char *data, uint32_t ms)
{
  // this is the data we want to appear in the advertisement
  // (the deviceName length plus the advertisement length must be <= 18 bytes
  RFduinoBLE.advertisementData = data;
  
  // start the BLE stack
  RFduinoBLE.begin();
  
  // advertise for ms milliseconds
  RFduino_ULPDelay(ms);
  
  // stop the BLE stack
  RFduinoBLE.end();
}

void setup()
{
  // led used to indicate that the RFduino is advertising
  pinMode(led, OUTPUT);
}

void loop()
{
  // start advertising
  digitalWrite(led, HIGH);
  
  // advertise "111" for indicated time
  advertise("111", duration);
  
  // adverise "222" for indicated time
  advertise("222", duration);

  // stop advertising 
  digitalWrite(led, LOW);
  
  // button used to wake up the RFduino (when button 1 is pressed, the RFduino will restart advertising)
  pinMode(button, INPUT);
  RFduino_pinWake(5, HIGH);
  
  // wait indefinitely in ultra low power and wake up on button 1 press
  RFduino_ULPDelay(INFINITE); 
  
  RFduino_resetPinWake(5);
}
