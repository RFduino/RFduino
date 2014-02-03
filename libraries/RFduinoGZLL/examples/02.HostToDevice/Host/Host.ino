/*
This sketch demonstrates how to send data from a Host
to a Device in a Gazell network.

Button A on the Host is used to toggle the Green led
on the Device.

Since the Device must initiate communication, the
device "polls" the Host evey 200ms.
*/

#include <RFduinoGZLL.h>

device_t role = HOST;

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

  delay_until_button(button_a, LOW);
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  // piggyback the current Host Button A state on the acknowledgement sent
  // back to the Device
  RFduinoGZLL.sendToDevice(device, state);
}
