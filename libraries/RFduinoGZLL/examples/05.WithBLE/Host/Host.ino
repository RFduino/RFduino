/*
This sketch demonstrates how to use Bluetooth Low Energy
and Gazell in the same sketch.

This sketch accepts a connection from the LedButton iPhone App.
When the button on the iPhone app is pressed, the green led on
the Host will toggle.
*/

#include <RFduinoGZLL.h>

device_t role = HOST;

// pin for the Green Led
int green_led = 3;

void setup()
{
  pinMode(green_led, OUTPUT);

  // start the GZLL stack  
  RFduinoGZLL.begin(role);
}

void loop()
{
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  char state = data[0];
  digitalWrite(green_led, state);
}
