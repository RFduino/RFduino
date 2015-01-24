/*
This sketch demonstrates how to send data from a Device
to a Host in a Gazell network.

When Button A on the Device is pressed and released,
the green led on the host will toggle.
*/

#include <RFduinoGZLL.h>

device_t role = DEVICE0;

// pin for Button A on the RGB Shield
int button_a = 5;

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

// starting state is off
char state = 0;

void setup()
{
  pinMode(button_a, INPUT);

  // use the lowest power level
  RFduinoGZLL.txPowerLevel = -20;

  // the host/device base address can be changed to create independent networks in the same area
  // (note: the msb cannot be 0x55 or 0xaa)
  RFduinoGZLL.hostBaseAddress = 0x12345678;    // default host base address is 0x0D0A0704;
  RFduinoGZLL.deviceBaseAddress = 0x87654321;  // default device base address is 0x0E0B0805;
  
  // start the GZLL stack
  RFduinoGZLL.begin(role);
}

int debounce(int button, int state)
{
  int start = millis();
  int debounce_start = start;
  
  while (millis() - start < debounce_timeout)
    if (digitalRead(button) == state)
    {
      if (millis() - debounce_start >= debounce_time)
        return 1;
    }
    else
      debounce_start = millis();

  return 0;
}

int delay_until_button(int button, int state)
{
  while (! debounce(button, state))
    ;
}

void loop()
{
  delay_until_button(button_a, HIGH);
  
  // toggle state
  state = ! state;

  // send state to Host
  RFduinoGZLL.sendToHost(state);
  
  delay_until_button(button_a, LOW);
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
}
