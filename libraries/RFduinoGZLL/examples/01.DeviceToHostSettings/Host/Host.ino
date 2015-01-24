/*
This sketch demonstrates how to send data from a Device
to a Host in a Gazell network.

The host and upto 3 devices should have the RGB shield
attached.  When Button A on a Device is pressed, the
associated led on the Host will toggle.  Device1 is
associated with the Red led, Device2 with the Green led
and Device3 with the Blue led.

The Green led on the Device will blink to indicate
that an acknowledgement from the Host was received.
*/

#include <RFduinoGZLL.h>

device_t role = HOST;

// pin for the Green Led
int green_led = 3;

void setup()
{
  pinMode(green_led, OUTPUT);

  // use the lowest power level
  RFduinoGZLL.txPowerLevel = -20;

  // the host/device base address can be changed to create independent networks in the same area
  // (note: the msb cannot be 0x55 or 0xaa)
  RFduinoGZLL.hostBaseAddress = 0x12345678;    // default host base address is 0x0D0A0704;
  RFduinoGZLL.deviceBaseAddress = 0x87654321;  // default device base address is 0x0E0B0805;
  
  // start the GZLL stack  
  RFduinoGZLL.begin(role);
}

void loop()
{
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  char state = data[0];

  // this test is not needed for a single device
  if (device == DEVICE0)
    digitalWrite(green_led, state);

  // no data to piggyback on the acknowledgement sent back to the Device
  // RFduinoGZLL.sendToDevice(device, "OK");
}
