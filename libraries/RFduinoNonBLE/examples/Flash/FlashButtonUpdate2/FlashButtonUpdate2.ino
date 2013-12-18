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
This sketch demonstrates another technique for updating
flash memory over multiple restarts of the RFduino.
*/

// pin 3 on the RGB shield is the green led
int led = 3;

// pin 5 on the RGB shield is button 1
int button = 5;

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

// magic number unnecessary because flash cannot be uninitialized
struct data_t
{
  int count;
};

// the flash page is dynamic (uses the first available flash page after the last sketch page)
// use __attribute__((aligned(1024)) to push a variable onto a separate flash page
// note: the flash page will be reset every time the code is uploaded
__attribute__((section(".flash")))
struct data_t value =
{ 
  0
};

void setup() {
  Serial.begin(57600);
  
  // led turned on/off when the button is pressed to indicate
  // flash memory was incremented
  pinMode(led, OUTPUT);

  // button to increment flash memory
  pinMode(button, INPUT);
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
  Serial.println(value.count);
  
  delay_until_button(HIGH);
  digitalWrite(led, HIGH);

  // make a temporary copy of value in RAM
  struct data_t temp = value;
  
  // update temporary copy
  temp.count++;

  // erase the flash page  
  flashPageErase(PAGE_FROM_ADDRESS(&value));  
  
  // write updated temporary copy back to flash
  flashWriteBlock(&value, &temp, sizeof(temp)); 
  
  delay_until_button(LOW);
  digitalWrite(led, LOW);
}

