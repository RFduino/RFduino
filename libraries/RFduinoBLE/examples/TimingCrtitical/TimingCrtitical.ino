/*
The sketch demonstrates how to integrate timing critical code
with Bluetooth Low Energy 4.
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

// flag used to toggle the led
bool flag = false;

void setup() {
  // led used to indicate that the RFduino is advertising
  pinMode(led, OUTPUT);

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "data";
  
  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
  // pause while the radio is active
  while (RFduinoBLE.radioActive)
    ;
    
  // timing critial code that must not be interrupted by radio events
  flag = ! flag;
  if (flag)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  // an artifical delay to simulate something more realistic
  delay(1);
}

