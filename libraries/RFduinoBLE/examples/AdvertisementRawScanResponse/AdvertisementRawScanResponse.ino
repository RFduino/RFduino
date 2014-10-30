/*
Demonstrates advanced raw access to the advertisement and scan response packets.

This example only applies if you have a specific need for raw access to the advertisement
packet.  This is for advanced use only, as the RFduino will not advertise is the packet
is invalid.

Note:
This advertisement packet uses a custom 128 bit UUID.
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

// the advertisement packet is composed of a series of variable length blocks, that can appear in any order.
// each block starts with a length byte, followed by a type byte, followed by the data.
// the payload cannot exceed 31 bytes.

uint8_t advdata[] =
{
  0x05,  // length
  0x09,  // complete local name type
  0x41,  // 'A'
  0x42,  // 'B'
  0x43,  // 'C'
  0x44,  // 'D'
  
  0x02,  // length
  0x01,  // flags type
  0x06,  // le general discovery mode | br edr not supported

  0x02,  // length
  0x0A,  // tx power level
  0x04,  // +4dBm
};

uint8_t srdata[] =
{
  0x11,    // length
  0x07,    // 128 bit service uuid (complete)
  
  // 128 bit uuid
  // equivalent to RFduinoBLE.customUUID = "c97433f0-be8f-4dc8-b6f0-5343e6100eb4"
  // little endian
  0xB4,
  0x0E,
  0x10,
  0xE6,
  0x43,
  0x53,
  0xF0,
  0xB6,
  0xC8,
  0x4D,
  0x8F,
  0xBE,
  0xF0,
  0x33,
  0x74,
  0xC9,
};

// pin 3 on the RGB shield is the green led
int led = 3;

void setup() {
  // led used to indicate that the RFduino is advertising
  pinMode(led, OUTPUT);

  RFduinoBLE_advdata = advdata;
  RFduinoBLE_advdata_len = sizeof(advdata);

  RFduinoBLE_srdata = srdata;
  RFduinoBLE_srdata_len = sizeof(srdata);

  // uncomment for nonconnectable advertising
  // (nonconnectable forces a minimum 100ms advertisement_interval, >= 100ms intervals are okay)
  // RFduinoBLE.connectable = false;
  
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
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
}
