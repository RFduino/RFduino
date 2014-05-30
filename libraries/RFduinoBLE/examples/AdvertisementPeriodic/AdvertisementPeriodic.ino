/*
The sketch demonstrates how to do power conservative Bluetooth Low Energy 4
Advertisement with the RFduino.  Advertisement of "111" is done for an
interval in seconds, then advertisement of "222" is done for the same
interval, then the RFduino is switched ultra low power mode.  The RFduino
can be woken up by pressing Button 1, at which time the device will
advertise again.
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
int led = 3;

// pin 5 on the RGB shield is button 1
int button = 5;

// time to advertise in milliseconds
int duration = SECONDS(5);

void advertise(const char *data, uint32_t ms)
{
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = data;
  
  // start the BLE stack
  RFduinoBLE.begin();
  
  // advertise for ms milliseconds
  RFduino_ULPDelay(ms);
  
  // stop the BLE stack
  RFduinoBLE.end();
}

void setup()
{
  // led used to indicate that the RFduino is advertising
  pinMode(led, OUTPUT);
}

void loop()
{
  // start advertising
  digitalWrite(led, HIGH);
  
  // advertise "111" for indicated time
  advertise("111", duration);
  
  // adverise "222" for indicated time
  advertise("222", duration);

  // stop advertising 
  digitalWrite(led, LOW);
  
  // button used to wake up the RFduino (when button 1 is pressed, the RFduino will restart advertising)
  pinMode(button, INPUT);
  RFduino_pinWake(5, HIGH);
  
  // wait indefinitely in ultra low power and wake up on button 1 press
  RFduino_ULPDelay(INFINITE); 
  
  RFduino_resetPinWake(5);
}
