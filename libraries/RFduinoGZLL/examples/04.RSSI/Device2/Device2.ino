/*
This sketch demonstrates how to coordinate data
between 3 devices in a Gazell network.

The host collects RSSI samples from the Devices,
and determines which device has the strongest
average RSSI (ie: the Device that is closest
to the Host).  The Green led is set on the
closest Device.

Since the Device must initiate communication, the
device "polls" the Host evey 200ms.
*/

#include <RFduinoGZLL.h>

// Gazell Role Assignment
device_t role = DEVICE2;

// pin for the Green Led
int green_led = 3;

void setup()
{
  pinMode(green_led, OUTPUT);

  RFduinoGZLL.txPowerLevel = 0;

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
    // set the Green led if this device is the closest device
    device_t closest_device = (device_t)data[0];
    digitalWrite(green_led, (role == closest_device));
  }
}
