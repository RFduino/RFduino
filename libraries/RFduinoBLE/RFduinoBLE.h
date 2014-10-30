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

#ifndef _RFDUINOBLE_H_
#define _RFDUINOBLE_H_

#include <stdint.h>
#include <stdbool.h>

// needed for callback support
#include "libRFduinoBLE.h"

class RFduinoBLEClass
{
public:
  // default "" (previous fixed RFduino uuid)
  const char *customUUID;

  // default "RFduino"
  const char *deviceName;

  // default "sketch"
  const char *advertisementData;

  bool iBeacon;
  uint8_t iBeaconUUID[16];
  unsigned short iBeaconMajor;
  unsigned short iBeaconMinor;
  // default 2's complement iBeacon Power Measurement at 1 meter (default is 0xC5 = -59dBm)
  unsigned char iBeaconMeasuredPower;

  // in ms (converted to 0.625ms units) - range 20ms to 10.24s - default 80ms
  int advertisementInterval;

  // -20 dBm to +4 dBm - default +4 dBm
  int txPowerLevel;

  // default true (false = nonconnectable advertising)
  // nonconnectable forces minimum of 100ms advertisement_interval
  bool connectable;

  // true if the radio is active (or about to become active), false if not
  static volatile int& radioActive;

public:
  RFduinoBLEClass();

  int begin();
  void end();

  bool send(char data);
  bool send(const char *data, int len);

  bool sendByte(uint8_t data);
  bool sendInt(int data);
  bool sendFloat(float data);
  
  // request central role use a different connection interval in the given range
  // the central role may reject the request (or even pick a value outside the range)
  void updateConnInterval(int min_conn_interval_ms, int max_conn_interval_ms);

  int getConnInterval(void);
};

extern RFduinoBLEClass RFduinoBLE;

#endif
