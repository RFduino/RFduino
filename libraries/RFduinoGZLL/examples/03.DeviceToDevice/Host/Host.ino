/*
This sketch demonstrates how to send data from a Device
to another Device (using the Host as an intermediary)
in a Gazell network.

When Button A on Device0 is pressed and released,
the green led on Device1 will toggle.
*/

#include <RFduinoGZLL.h>

device_t role = HOST;

// the last known state from DEVICE0 (default to off)
char state = 0;

void setup()
{
  // start the GZLL stack  
  RFduinoGZLL.begin(role);
}

void loop()
{
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  if (device == DEVICE0)  // get the current state from DEVICE0
    state = data[0];
  else if (device == DEVICE1)  // relay the last known state to DEVICE1
    RFduinoGZLL.sendToDevice(device, state);
}
