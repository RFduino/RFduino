/*
This sketch demonstrates how to send data from a Host
to a Device in a Gazell network.

Button A on the Host is used to toggle the Green led
on the Device.

Since the Device must initiate communication, the
device "polls" the Host evey 200ms.
*/

#include <RFduinoGZLL.h>

device_t role = DEVICE0;

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
    // update the Green led with the current state on the Host
    int state = data[0];
    digitalWrite(green_led, state);
  }
}
