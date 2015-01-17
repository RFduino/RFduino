/*
 RFduinoGZLL.h

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

/*
  Copyright (c) 2012, Nordic Semiconductor.  All Rights Reserved.

  Gazell is the property of Nordic Semiconductor ASA.
  Terms and conditions of the usage are described in detail in NORDIC
  SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
*/

/*
The Gazell protocol is a wireless communication protocol that is used
to setup a robust wireless link between a single host and up to eight
devices in a star network topology.

The Host in a Gazell network is always listening, and it is the Device
that always initiates a communication.

Each packet that a Device sends is required to be acknowledged by the
Host.  Gazell auotmatically handle packet retransmission if necessary.

It is possible for the Host to send data to the Device by piggybacking
data to an acknowledgement (ACK) packet.  Therefore a Host has to wait
for a packet from a Device before it can send any data to it.

Gazell utilizes channel hopping functionality that gives a high date
rate and reliable wireless link.

Gazell requires no connection packets to setup a link, and devices can
enter and exit from the network at any time.
*/

#ifndef _RFDUINOGZLL_H_
#define _RFDUINOGZLL_H_

#include <stdint.h>
#include <stdbool.h>

#include "WString.h"

// needed for enum and callback support
#include "libRFduinoGZLL.h"

class RFduinoGZLLClass
{
public:
  // -20 dBm to +4 dBm - default +4 dBm
  int txPowerLevel;
  uint32_t hostBaseAddress;
  uint32_t deviceBaseAddress;

public:
  RFduinoGZLLClass();

  int begin(device_t device);
  void end();

  // max length is 32 bytes
  bool sendToHost(const char *data, int len);
  bool sendToDevice(device_t device, const char *data, int len);

  bool sendToHost(char data)          { return sendToHost((const char *)&data, sizeof(char)); }
  bool sendToHost(unsigned char data) { return sendToHost((const char *)&data, sizeof(unsigned char)); }
  bool sendToHost(int data)           { return sendToHost((const char *)&data, sizeof(int)); }
  bool sendToHost(unsigned int data)  { return sendToHost((const char *)&data, sizeof(unsigned int)); }
  bool sendToHost(long data)          { return sendToHost((const char *)&data, sizeof(long)); }
  bool sendToHost(unsigned long data) { return sendToHost((const char *)&data, sizeof(unsigned long)); }
  bool sendToHost(float data)         { return sendToHost((const char *)&data, sizeof(float)); }
  bool sendToHost(double data)        { return sendToHost((const char *)&data, sizeof(double)); }
  bool sendToHost(char *data)         { return sendToHost((const char *)data); }
  bool sendToHost(const char *data);
  bool sendToHost(String &data);
  bool sendToHost(const __FlashStringHelper *data) { return sendToHost(reinterpret_cast<const char *>(data)); }

  bool sendToDevice(device_t device, char data)          { return sendToDevice(device, (const char *)&data, sizeof(char)); }
  bool sendToDevice(device_t device, unsigned char data) { return sendToDevice(device, (const char *)&data, sizeof(unsigned char)); }
  bool sendToDevice(device_t device, int data)           { return sendToDevice(device, (const char *)&data, sizeof(int)); }
  bool sendToDevice(device_t device, unsigned int data)  { return sendToDevice(device, (const char *)&data, sizeof(unsigned int)); }
  bool sendToDevice(device_t device, long data)          { return sendToDevice(device, (const char *)&data, sizeof(long)); }
  bool sendToDevice(device_t device, unsigned long data) { return sendToDevice(device, (const char *)&data, sizeof(unsigned long)); }
  bool sendToDevice(device_t device, float data)         { return sendToDevice(device, (const char *)&data, sizeof(float)); }
  bool sendToDevice(device_t device, double data)        { return sendToDevice(device, (const char *)&data, sizeof(double)); }
  bool sendToDevice(device_t device, char *data)         { return sendToDevice(device, (const char *)data); }
  bool sendToDevice(device_t device, const char *data);
  bool sendToDevice(device_t device, String &data);
  bool sendToDevice(device_t device, const __FlashStringHelper *data) { return sendToDevice(device, reinterpret_cast<const char *>(data)); }
};

extern RFduinoGZLLClass RFduinoGZLL;

#endif
