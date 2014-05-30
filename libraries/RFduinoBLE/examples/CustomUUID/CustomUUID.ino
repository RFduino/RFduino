/*
The sketch demonstrates how to use a different UUID for
the RFduino service and characteristics.

This example only applies if you have a specific
need to use a custom 128 bit uuid.

Note:
If you use a customUUID, the iPhone apps must also
be updated to discover RFduino's advertising with the
custom UUID.

The iPhone application source code in this distribution
has been enhanced to discover RFduino's with customUUIDs,
by declaring the UUID in the +Load method of the
AppViewController.m.  Without modification, the iPhone
apps continue to operate as before.

The RFduino AppStore apps will continue to only discover
RFduino's that advertise using the previous fixed RFduino
UUID.
*/

/*
 Copyright (c) 2014 OpenSourceRF.com.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <RFduinoBLE.h>

// pin 3 on the RGB shield is the green led
// (shows when the RFduino is advertising or not)
int advertisement_led = 3;

// pin 2 on the RGB shield is the red led
// (goes on when the RFduino has a connection from the iPhone, and goes off on disconnect)
int connection_led = 2;

void setup() {
  // led used to indicate that the RFduino is advertising
  pinMode(advertisement_led, OUTPUT);
  
  // led used to indicate that the RFduino is connected
  pinMode(connection_led, OUTPUT);

  // 128 bit base uuid
  // (generated with http://www.uuidgenerator.net)
  RFduinoBLE.customUUID = "c97433f0-be8f-4dc8-b6f0-5343e6100eb4";

  // this must match the customUUID in the iPhone app declared
  // in +Load method of AppViewController.m
  
  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
  // switch to lower power mode
  RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onAdvertisement(bool start)
{
  // turn the green led on if we start advertisement, and turn it
  // off if we stop advertisement
  
  if (start)
    digitalWrite(advertisement_led, HIGH);
  else
    digitalWrite(advertisement_led, LOW);
}

void RFduinoBLE_onConnect()
{
  digitalWrite(connection_led, HIGH);
}

void RFduinoBLE_onDisconnect()
{
  digitalWrite(connection_led, LOW);
}
