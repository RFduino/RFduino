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

/*
The sketch demonstrates various Serial.printf() calls.
*/

void setup() {
  char ch = 'A';
  int i = -123;
  double d = -123.456;
   
  Serial.begin(57600);
  
  Serial.printf("Hello World!\n");
  Serial.printf("ch = %c\n", ch);
  Serial.printf("i = %d, unsigned = %u, hex = %x, binary = %b\n", i, i, i, i);
  Serial.printf("d = %f\n", d);
  Serial.printf("string1 = %s, string2 = %s\n", "hi", F("hi"));

  // this works, but has been commented out for testing due to String impact on code size
  // Serial.printf("string3 = %s\n", String("hi").cstr());  
}

void loop() {
}
