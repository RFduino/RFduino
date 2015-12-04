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
  Copyright (c) 2011 Arduino.  All right reserved.

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
*/

#ifndef _WIRING_CONSTANTS_
#define _WIRING_CONSTANTS_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define INPUT_NOPULL 0x3
#define INPUT_PULLDOWN 0x4
#define OUTPUT_S0S1 0x5
#define OUTPUT_H0S1 0x6
#define OUTPUT_S0H1 0x7
#define OUTPUT_H0H1 0x8
#define OUTPUT_D0S1 0x9
#define OUTPUT_D0H1 0xA
#define OUTPUT_S0D1 0xB
#define OUTPUT_H0D1 0xC

#define true 0x1
#define false 0x0

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define SERIAL  0x0
#define DISPLAY 0x1

enum BitOrder {
	LSBFIRST = 0,
	MSBFIRST = 1
};

#define LOW 0
#define HIGH 1
#define CHANGE 2
#define FALLING 3
#define RISING 4

// ADC reference selection
#define DEFAULT 	ADC_CONFIG_REFSEL_SupplyOneThirdPrescaling	// Use VDD with 1/3 prescaling. (Only applicable when VDD is in the range 2.5V – 3.6V)
#define EXTERNAL 	ADC_CONFIG_REFSEL_External					// Use external reference specified by CONCFIG EXTREFSEL

#define VBG			ADC_CONFIG_REFSEL_VBG						// Use internal 1.2 V band gap reference
#define EXT			ADC_CONFIG_REFSEL_External					// Use external reference specified by CONCFIG EXTREFSEL
#define VDD_1_2_PS	ADC_CONFIG_REFSEL_SupplyOneHalfPrescaling	// Use VDD with 1/2 prescaling. (Only applicable when VDD is in the range 1.7V – 2.6V)
#define VDD_1_3_PS	ADC_CONFIG_REFSEL_SupplyOneThirdPrescaling	// Use VDD with 1/3 prescaling. (Only applicable when VDD is in the range 2.5V – 3.6V)

// External reference pin selection
#define DEFAULT_EXT_REF		ADC_CONFIG_EXTREFSEL_AnalogReference0		// Use AREF0 as analog reference

#define NONE		ADC_CONFIG_EXTREFSEL_None					// Analog reference inputs disabled
#define AREF0		ADC_CONFIG_EXTREFSEL_AnalogReference0		// Use AREF0 as analog reference
#define AREF1		ADC_CONFIG_EXTREFSEL_AnalogReference1		// Use AREF1 as analog reference

// ADC input selection
#define DEFAULT_INPUT_SEL	ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling 		// Analog input pin specified by CONFIG. PSEL with 1/3 prescaling

#define AIN_NO_PS			ADC_CONFIG_INPSEL_AnalogInputNoPrescaling 				// Analog input pin specified by CONFIG.PSEL with no prescaling
#define AIN_2_3_PS			ADC_CONFIG_INPSEL_AnalogInputTwoThirdsPrescaling		// Analog input pin specified by CONFIG. PSEL with 2/3 prescaling
#define AIN_1_3_PS			ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling 		// Analog input pin specified by CONFIG. PSEL with 1/3 prescaling
#define VDD_2_3_PS			ADC_CONFIG_INPSEL_SupplyTwoThirdsPrescaling				// Supply voltage with 2/3 prescaling used as input for the conversion
#define VDD_1_3_PS			ADC_CONFIG_INPSEL_SupplyOneThirdPrescaling				// Supply voltage with 1/3 prescaling used as input for the conversion

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif // min

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif // max

#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef uint8_t boolean ;
typedef uint8_t byte ;


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* _WIRING_CONSTANTS_ */
