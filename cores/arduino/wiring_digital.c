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

#include "Arduino.h"

#ifdef __cplusplus
 extern "C" {
#endif

void pinMode( uint32_t ulPin, uint32_t ulMode )
{
	if (ulPin < PINS_COUNT)
	{
		switch ( ulMode )
		{
			case INPUT:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case INPUT_NOPULL:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case INPUT_PULLDOWN:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case INPUT_PULLUP:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_S0S1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_H0S1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_H0S1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_S0H1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_S0H1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_H0H1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_D0S1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_D0S1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_D0H1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_D0H1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_S0D1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;

			case OUTPUT_H0D1:
				NRF_GPIO->PIN_CNF[ulPin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
										| (GPIO_PIN_CNF_DRIVE_H0D1 << GPIO_PIN_CNF_DRIVE_Pos)
										| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
										| (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
										| (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
			break ;
		}
	}
}

void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
	// stops pwm
	if (Pin_Occupied_for_PWM[ulPin])
	{
		// Disable GPIOTE and PPI channels from GPIO
		turn_Off_GPIOTE_PPI_from_GPIO(ulPin);
		Pin_Occupied_for_PWM[ulPin] = 0;
	}

  if (ulVal)
    NRF_GPIO->OUTSET = (1 << ulPin);
  else
    NRF_GPIO->OUTCLR = (1 << ulPin);
}

int digitalRead( uint32_t ulPin )
{
	// stops pwm
	if (Pin_Occupied_for_PWM[ulPin])
	{
		// Disable GPIOTE and PPI channels from GPIO
		turn_Off_GPIOTE_PPI_from_GPIO(ulPin);
		Pin_Occupied_for_PWM[ulPin] = 0;
	}

  return ((NRF_GPIO->IN >> ulPin) & 1UL);
}

#ifdef __cplusplus
}
#endif
