/*
 RFduinoGZLL.cpp

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

#include "Arduino.h"
#include "RFduinoGZLL.h"

RFduinoGZLLClass::RFduinoGZLLClass()
{
  ::RFduinoGZLL_used = 1;

	txPowerLevel = +4;
	hostBaseAddress = 0U;
	deviceBaseAddress = 0U;
}

int RFduinoGZLLClass::begin(device_t device)
{
  RFduinoGZLL_tx_power_level = txPowerLevel;
  RFduinoGZLL_host_base_address   = hostBaseAddress;
  RFduinoGZLL_device_base_address = deviceBaseAddress;

  return RFduinoGZLL_begin(device);
}

void RFduinoGZLLClass::end()
{
  RFduinoGZLL_end();
  NRF_RADIO->TASKS_RSSISTOP = 1;
}

bool RFduinoGZLLClass::sendToHost(const char *data, int len)
{
	return RFduinoGZLL_send_to_host(data, len);
}

bool RFduinoGZLLClass::sendToDevice(device_t device, const char *data, int len)
{
  return RFduinoGZLL_send_to_device(device, data, len);
}

bool RFduinoGZLLClass::sendToHost(const char *data)
{
  return sendToHost(data, strlen(data));
}

bool RFduinoGZLLClass::sendToDevice(device_t device, const char *data)
{
  return sendToDevice(device, data, strlen(data));
}

bool RFduinoGZLLClass::sendToHost(String &data)
{
  char buf[32];
  data.toCharArray(buf, sizeof(buf));
  return sendToHost(buf);
}

bool RFduinoGZLLClass::sendToDevice(device_t device, String &data)
{
  char buf[32];
  data.toCharArray(buf, sizeof(buf));
  return sendToDevice(device, buf);
}

RFduinoGZLLClass RFduinoGZLL;
