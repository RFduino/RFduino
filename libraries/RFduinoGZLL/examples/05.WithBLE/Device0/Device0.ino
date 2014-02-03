/*
This sketch demonstrates how to use Bluetooth Low Energy
and Gazell in the same sketch.

This sketch accepts a connection from the LedButton iPhone App.
When the button on the iPhone app is pressed, the green led on
the Host will toggle.
*/

#include <RFduinoGZLL.h>
#include <RFduinoBLE.h>

device_t role = DEVICE0;

char led = 0;

int state = 0;

void setup()
{
  Serial.begin(57600);
  Serial.println("start");  
}

void loop()
{
  switch (state)
  {
  case 0:
    RFduinoBLE.begin();
    state = 1;
    break;
    
  case 1:
    // waiting for a BLE button press
    break;
    
  case 2:
    // this disconnects us from the iPhone App
    // (we get a warning on the iPhone App because we are
    // not detecting this condition correctly in the App)
    RFduinoBLE.end();
    state = 3;
    break;
    
  case 3:
    RFduinoGZLL.begin(role);
    RFduinoGZLL.sendToHost(led);
    state = 4;
    break;
 
  case 4:
    // waiting for GZLL acknowledgement from Host
    break;
 
  case 5:
    RFduinoGZLL.end();
    state = 0;
    break;
  }
}

void RFduinoBLE_onReceive(char *data, int len)
{
  led = ! led;
  state = 2;
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  state = 5;
}
