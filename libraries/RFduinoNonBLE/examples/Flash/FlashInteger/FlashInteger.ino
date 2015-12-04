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

/*
This sketch demonstrates how to read and write an unsigned
32-bit integer to flash memory on the RFduino.

This example is located under RFduinoNonBLE, but the same
code can be used with Bluetooth Low Energy 4 functionality.
*/

// select a flash page that isn't in use (see Memory.h for more info)
#define  MY_FLASH_PAGE  251
#define  MY_FLASH_VALUE  0x12345678

// double level of indirection required to get gcc
// to apply the stringizing operator correctly
#define  str(x)   xstr(x)
#define  xstr(x)  #x

void setup() {
  Serial.begin(38400);
  Serial.println("All output will appear on the serial monitor.");

  // a flash page is 1K in length, so page 251 starts at address 251 * 1024 = 257024 = 3EC00 hex
  uint32_t *p = ADDRESS_OF_PAGE(MY_FLASH_PAGE);
  int rc;
   
  Serial.print("The value stored in flash page " str(MY_FLASH_PAGE) " is: 0x");
  Serial.println(*p, HEX);
  
  Serial.print("Attempting to erase flash page " str(MY_FLASH_PAGE) ": ");
  rc = flashPageErase(MY_FLASH_PAGE);
  if (rc == 0)
    Serial.println("Success");
  else if (rc == 1)
    Serial.println("Error - the flash page is reserved");
  else if (rc == 2)
    Serial.println("Error - the flash page is used by the sketch");
   
  Serial.print("The value stored in flash page " str(MY_FLASH_PAGE) " is: 0x");
  Serial.println(*p, HEX);
  
  Serial.print("Attempting to store " str(MY_FLASH_VALUE) " in flash page " str(MY_FLASH_PAGE) ": ");
  rc = flashWrite(p, MY_FLASH_VALUE);
  if (rc == 0)
    Serial.println("Success");
  else if (rc == 1)
    Serial.println("Error - the flash page is reserved");
  else if (rc == 2)
    Serial.println("Error - the flash page is used by the sketch");
    
  Serial.print("The value stored in flash page " str(MY_FLASH_PAGE) " is: 0x");
  Serial.println(*p, HEX);
}

void loop() {
}

