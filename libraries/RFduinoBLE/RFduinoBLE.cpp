/*
 Copyright (c) 2013 OpenSourceRF.com.  All right reserved.

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

#include "Arduino.h"
#include "RFduinoBLE.h"

volatile int& RFduinoBLEClass::radioActive = RFduinoBLE_radioActive;

RFduinoBLEClass::RFduinoBLEClass()
{
  ::RFduinoBLE_used = 1;

	deviceName = "RFduino";
	advertisementData = "sketch";
	advertisementInterval = 80;
	txPowerLevel = +4;
}

int RFduinoBLEClass::begin()
{
  if (UART0_State != UART0_State_NotStarted && UART0_BaudRate() > 9600)
  {
    const char *error = "BLE + UART > 9600 baud not permitted due to critical BLE timing requirements";

    // attempt to notify user of error condition
    const char *p = error;
    while (*p)
      UART0_TX(*p++);

    // don't continue
    while (1)
      ;
  }

  RFduinoBLE_device_name = deviceName;
  RFduinoBLE_advertisement_data = advertisementData;
  RFduinoBLE_advertisement_interval = advertisementInterval;
  RFduinoBLE_tx_power_level = txPowerLevel;

  return RFduinoBLE_begin();
}

void RFduinoBLEClass::end()
{
  RFduinoBLE_end();
}

bool RFduinoBLEClass::send(char data)
{
	return RFduinoBLE_send(&data, 1);
}

bool RFduinoBLEClass::send(const char *data, int len)
{
	return RFduinoBLE_send(data, len);
}

bool RFduinoBLEClass::sendByte(uint8_t data)
{
  return RFduinoBLE_send((char*)&data, 1);
}

bool RFduinoBLEClass::sendInt(int data)
{
  return RFduinoBLE_send((char*)&data, sizeof(int));
}

bool RFduinoBLEClass::sendFloat(float data)
{
  return RFduinoBLE_send((char*)&data, sizeof(float));
}

RFduinoBLEClass RFduinoBLE;
