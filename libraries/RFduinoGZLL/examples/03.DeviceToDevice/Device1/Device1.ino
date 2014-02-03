/*
This sketch demonstrates how to send data from a Device
to another Device (using the Host as an intermediary)
in a Gazell network.

When Button A on Device0 is pressed and released,
the green led on Device1 will toggle.
*/

#include <RFduinoGZLL.h>

device_t role = DEVICE1;

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
  delay(200);

  // request the state from the Host (send a 0 byte payload)
  RFduinoGZLL.sendToHost(NULL, 0);  
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  // ignore acknowledgement without payload
  if (len > 0)
  {
    // update the Green led with the current state relayed by the Host
    char state = data[0];
    digitalWrite(green_led, state);
  }
}
