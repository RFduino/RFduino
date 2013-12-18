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
This sketch demonstrates updating flash memory over multiple
restarts of the RFduino.
*/

// select a flash page that isn't in use (see Memory.h for more info)
#define  MY_FLASH_PAGE  251

// pin 3 on the RGB shield is the green led
int led = 3;

// pin 5 on the RGB shield is button 1
int button = 5;

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

struct data_t
{
  // we will use java's famous 0xCAFEBABE magic number to indicate
  // that the flash page has been initialized previously by our
  // sketch
  int magic_number;
  int count;
};

struct data_t *flash = (data_t*)ADDRESS_OF_PAGE(MY_FLASH_PAGE);

void flashSave(int count)
{
  flashPageErase(MY_FLASH_PAGE);
    
  struct data_t value;
  value.magic_number = 0xCAFEBABE;
  value.count = count;

  flashWriteBlock(flash, &value, sizeof(value)); 
}

void setup() {
  Serial.begin(57600);
  
  // led turned on/off when the button is pressed to indicate
  // flash memory was incremented
  pinMode(led, OUTPUT);

  // button to increment flash memory
  pinMode(button, INPUT);
  
  // if flash page is not initialized, initialize it
  if (flash->magic_number != 0xCAFEBABE)
    flashSave(0);
}

int debounce(int state)
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

int delay_until_button(int state)
{
  // set button edge to wake up on
  if (state)
    RFduino_pinWake(button, HIGH);
  else
    RFduino_pinWake(button, LOW);
    
  do
    // switch to lower power mode until a button edge wakes us up
    RFduino_ULPDelay(INFINITE);
  while (! debounce(state));
  
  // if multiple buttons were configured, this is how you would determine what woke you up
  if (RFduino_pinWoke(button))
  {
    // execute code here
    RFduino_resetPinWake(button);
  }
}

void loop() {
  Serial.print("The current count value in flash is: ");
  Serial.println(flash->count);
  
  delay_until_button(HIGH);
  digitalWrite(led, HIGH);

  flashSave(flash->count + 1);
  
  delay_until_button(LOW);
  digitalWrite(led, LOW);
}

