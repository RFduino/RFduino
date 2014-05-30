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

int led_red = 2;
int led_green = 3;
int led_blue = 4;
int led_yellow = 5;
int i = 0;
int mode = 0;

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_yellow, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  
  analogWrite(led_red, i);
  analogWrite(led_green, (255 - i));
  analogWrite(led_blue, i);
  analogWrite(led_yellow, (255 - i));

  if (mode == 0)
  {
    if (++i >= 255)
    {
      mode = 1;
    }
  }
  else
  {
    if (--i == 0)
    {
      mode = 0;
    }
  }
  delay(25);
}

