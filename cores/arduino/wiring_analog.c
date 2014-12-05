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

static int _readResolution = 10;
static int _writeResolution = 8;

static uint32_t PWM_Channels_Value[4] = {(2^PWM_RESOLUTION - 1), (2^PWM_RESOLUTION - 1), (2^PWM_RESOLUTION - 1), (2^PWM_RESOLUTION - 1)};
static uint8_t PWM_Channels_Start[4] = {0, 0, 0, 0};

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
	if (from == to)
		return value;
	if (from > to)
		return value >> (from-to);
	else
		return value << (to-from);
}

uint8_t analog_reference = DEFAULT; // VDD with 1/3 prescaling
uint8_t external_reference = DEFAULT_EXT_REF; // AREF0
uint8_t adc_input_selection = DEFAULT_INPUT_SEL; // Analog input pin specified by CONFIG. PSEL with 1/3 prescaling

void analogReference(uint8_t ulMode)
{
	analog_reference = ulMode;
}

void externalReference(uint8_t ulMode)
{
	external_reference = ulMode;
}

void analogSelection(uint8_t ulMode)
{
	adc_input_selection = ulMode;
}

uint32_t analogRead(uint32_t ulPin)
{
	uint32_t ulValue = 0;
	uint8_t pselValue = 0;
	if ((ulPin > 0) && (ulPin < PINS_COUNT))	// Only pins 1-6 is avaliable for using as ADC inputs
	{
		pselValue = (1 << (ulPin + 1));	// Calculate PSEL value
    if (analog_reference == ADC_CONFIG_REFSEL_External)
		    NRF_ADC->CONFIG = 	(ADC_CONFIG_RES_10bit 	<< ADC_CONFIG_RES_Pos)|			// 10bit ADC resolution
							(adc_input_selection 	<< ADC_CONFIG_INPSEL_Pos)|		// DEFAULT: Analog input specified by PSEL with 1/3 prescaling used as input for the conversion
							(ADC_CONFIG_REFSEL_External  << ADC_CONFIG_REFSEL_Pos)|
							(pselValue 				<< ADC_CONFIG_PSEL_Pos)|		// Select ADC input
							(external_reference 	<< ADC_CONFIG_EXTREFSEL_Pos);	// DEFAULT: Use analog reference 0 as reference if selected external reference
    else
        NRF_ADC->CONFIG =   (ADC_CONFIG_RES_10bit   << ADC_CONFIG_RES_Pos)|     // 10bit ADC resolution
              (adc_input_selection  << ADC_CONFIG_INPSEL_Pos)|    // DEFAULT: Analog input specified by PSEL with 1/3 prescaling used as input for the conversion
              (analog_reference     << ADC_CONFIG_REFSEL_Pos)|    // DEFAULT: Use supply voltage with 1/3 prescaling as reference for conversion. Only usable when supply voltage is between 2.5V and 3.6V
              (pselValue        << ADC_CONFIG_PSEL_Pos)|    // Select ADC input
              (ADC_CONFIG_EXTREFSEL_None  << ADC_CONFIG_EXTREFSEL_Pos);
		NRF_ADC->INTENCLR = 0xFFFFFFFF;
		NRF_ADC->ENABLE = 	(ADC_ENABLE_ENABLE_Enabled 		<< ADC_ENABLE_ENABLE_Pos);		// Enable ADC
		NRF_ADC->TASKS_START = 	1;															// Start A-D conversion
    NRF_ADC->EVENTS_END = 0;
    while (! NRF_ADC->EVENTS_END)  // Wait for end of conversion
      ;
		ulValue = NRF_ADC->RESULT;															// Read the value
		ulValue = mapResolution(ulValue, ADC_RESOLUTION, _readResolution);
		NRF_ADC->ENABLE =	(ADC_ENABLE_ENABLE_Disabled 	<< ADC_ENABLE_ENABLE_Pos);		// Disable ADC
    NRF_ADC->TASKS_STOP = 1;
		// GPIOs release regarding PAN028
		NRF_ADC->CONFIG = 	(ADC_CONFIG_RES_8bit << ADC_CONFIG_RES_Pos) |
							(ADC_CONFIG_INPSEL_SupplyTwoThirdsPrescaling << ADC_CONFIG_INPSEL_Pos) |
							(ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos) |
							(ADC_CONFIG_PSEL_Disabled << ADC_CONFIG_PSEL_Pos) |
							(ADC_CONFIG_EXTREFSEL_None << ADC_CONFIG_EXTREFSEL_Pos);
	}
	return ulValue;
}

int find_free_PPI_channel(int exclude_channel)
{
  // For BLE / NonBLE, find first free PPI Channel in the range 0-7. Note the channels 8-15 is occupied by SoftDevice.  Maximum 4 PPI pairs.
  // For GZLL only, find first free PPI Channel in the range 3-8.  Note the channels 0-2 and TIMER2 are used by Gazell.  Maximum 3 PPI pairs.
  // For GZLL and BLE, find first free PPI Channel in the range 3-7.  Maximum 2 PPI pairs.
  int start = 0;
  int end = 8;

  int i;

  if (RFduinoGZLL_used)
    start = 3;

  if (! RFduinoBLE_used)
    end = 16;

  for (i = start; i < end; i++)
    if (! (NRF_PPI->CHEN & (1 << i)))
      if (i != exclude_channel)
        return i;

  return 255;
}

void turn_On_PPI_to_GPIO_for_PWM(uint32_t ulPin, uint32_t gpiote_channel, NRF_TIMER_Type* Timer, uint32_t CC_channel)
{
  // Initialize Programmable Peripheral Interconnect
  int chan_0 = find_free_PPI_channel(255);
  int chan_1 = find_free_PPI_channel(chan_0);

	if ((chan_0 != 255) && (chan_1 != 255))
	{
		// Configure PPI channel "chan_0" to toggle "ulPin" pin on every Timer COMPARE[CC_channel] match
    rfduino_ppi_channel_assign(chan_0, &((*Timer).EVENTS_COMPARE[CC_channel]), &NRF_GPIOTE->TASKS_OUT[gpiote_channel]);
  	// Save PPI channel number
  	PPI_Channels_Occupied[ulPin][0] = chan_0;

		// Configure PPI channel "chan_1" to toggle "ulPin" pin on every Timer COMPARE[3] match
    rfduino_ppi_channel_assign(chan_1, &((*Timer).EVENTS_COMPARE[3]), &NRF_GPIOTE->TASKS_OUT[gpiote_channel]);
		// Save PPI channel number
		PPI_Channels_Occupied[ulPin][1] = chan_1;
	}
}

// called from wiring_digital.c
void turn_Off_GPIOTE_PPI_from_GPIO(uint32_t ulPin)
{
  uint32_t i;

  if (PPI_Channels_Occupied[ulPin][0] < 255)
  {
    rfduino_ppi_channel_unassign(PPI_Channels_Occupied[ulPin][0]);
    PPI_Channels_Occupied[ulPin][0] = 255;
  }

  if (PPI_Channels_Occupied[ulPin][1] < 255)
  {
    rfduino_ppi_channel_unassign(PPI_Channels_Occupied[ulPin][1]);
    PPI_Channels_Occupied[ulPin][1] = 255;
  }

  if (GPIOTE_Channels_Occupied[ulPin] < 255)
  {
    nrf_gpiote_unconfig(GPIOTE_Channels_Occupied[ulPin]);
    GPIOTE_Channels_Occupied[ulPin] = 255;
  }

  for (i = 0; i < 3; i++)
  {
    if (Timer1_Compare_Unit_Occupied_by_Pin[i] == ulPin)
      Timer1_Compare_Unit_Occupied_by_Pin[i] = 255;
    if (Timer2_Compare_Unit_Occupied_by_Pin[i] == ulPin)
      Timer2_Compare_Unit_Occupied_by_Pin[i] = 255;
  }
}

static void update_PWM_value(uint32_t ulPin, uint32_t ulValue, uint32_t PWM_channel)
{
  PWM_Channels_Value[PWM_channel] = (2^PWM_RESOLUTION - 1) - mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
  if ((NRF_GPIOTE->CONFIG[PWM_channel] & GPIOTE_CONFIG_MODE_Msk) == (GPIOTE_CONFIG_MODE_Disabled << GPIOTE_CONFIG_MODE_Pos))
  {
    PWM_Channels_Start[PWM_channel] = 1;
  }
}

/** Timer 1 peripheral interrupt handler.
 */
static void TIMER1_Interrupt(void)
{
	// Update the CCx values
	NRF_TIMER1->CC[0] = PWM_Channels_Value[0];
	NRF_TIMER1->CC[1] = PWM_Channels_Value[1];
	NRF_TIMER1->CC[2] = PWM_Channels_Value[2];

	if (PWM_Channels_Start[0] == 1)
	{
		NRF_TIMER1->EVENTS_COMPARE[0] = 0;
		nrf_gpiote_task_config(0, Timer1_Compare_Unit_Occupied_by_Pin[0], NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
		PWM_Channels_Start[0] = 0;
	}
	if (PWM_Channels_Start[1] == 1)
	{
		NRF_TIMER1->EVENTS_COMPARE[1] = 0;
		nrf_gpiote_task_config(1, Timer1_Compare_Unit_Occupied_by_Pin[1], NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
		PWM_Channels_Start[1] = 0;
	}
	if (PWM_Channels_Start[2] == 1)
	{
		NRF_TIMER1->EVENTS_COMPARE[2] = 0;
		nrf_gpiote_task_config(2, Timer1_Compare_Unit_Occupied_by_Pin[2], NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
		PWM_Channels_Start[2] = 0;
	}

	NRF_TIMER1->EVENTS_COMPARE[3] = 0;
}

/** Timer 2 peripheral interrupt handler.
 */
static void TIMER2_Interrupt(void)
{
	// Update the CCx values
	NRF_TIMER2->CC[0] = PWM_Channels_Value[3];

	if (PWM_Channels_Start[3] == 1)
	{
		nrf_gpiote_task_config(3, Timer2_Compare_Unit_Occupied_by_Pin[0], NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
		PWM_Channels_Start[3] = 0;
	}

	NRF_TIMER2->EVENTS_COMPARE[3] = 0;
}

void analogWrite(uint32_t ulPin, uint32_t ulValue) {

  if (ulValue == 0)
  {
    digitalWrite(ulPin, LOW);
    return;
  }

  if (ulValue == 255)
  {
    digitalWrite(ulPin, HIGH);
    return;
  }

	if (Timer1_Compare_Unit_Occupied_by_Pin[0] == ulPin)
	{
		update_PWM_value(ulPin, ulValue, 0);
	}
	else if (Timer1_Compare_Unit_Occupied_by_Pin[1] == ulPin)
	{
		update_PWM_value(ulPin, ulValue, 1);
	}
	else if (Timer1_Compare_Unit_Occupied_by_Pin[2] == ulPin)
	{
		update_PWM_value(ulPin, ulValue, 2);
	}
	else if (Timer2_Compare_Unit_Occupied_by_Pin[0] == ulPin)
	{
		update_PWM_value(ulPin, ulValue, 3);
	}
	else
	{
		if ((Timer1_Compare_Unit_Occupied_by_Pin[0] == 255) && (Timer1_Compare_Unit_Occupied_by_Pin[1] == 255) && (Timer1_Compare_Unit_Occupied_by_Pin[2] == 255))
		{
			// Timer1 is not used: need to initialize it

			// Configure ulPin as output
      digitalWrite(ulPin, LOW);
			pinMode(ulPin, OUTPUT);

			if (ulValue > 0)
			{
				// Configure GPIOTE channel "gpiote_channel" to toggle the PWM pin state
				// Note that we can only connect one GPIOTE task to an output pin
				nrf_gpiote_task_config(0, ulPin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
			}
			GPIOTE_Channels_Occupied[ulPin] = 0;

			NRF_TIMER1->TASKS_STOP = 1;
			NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
			//NRF_TIMER1->PRESCALER = 6; // Source clock frequency is divided by 2^6 = 64
			NRF_TIMER1->PRESCALER = 0; // Source clock frequency is divided by 2^6 = 64 /////////////////////////////////////////
			//NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
			NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	////////////////////////////
			// Clears the timer, sets it to 0
			NRF_TIMER1->TASKS_CLEAR = 1;
			NRF_TIMER1->CC[0] = (2^PWM_RESOLUTION - 1);
			NRF_TIMER1->CC[1] = (2^PWM_RESOLUTION - 1);
			NRF_TIMER1->CC[2] = (2^PWM_RESOLUTION - 1);
			NRF_TIMER1->CC[3] = 0;
			NRF_TIMER1->EVENTS_COMPARE[0] = 0;
			NRF_TIMER1->EVENTS_COMPARE[1] = 0;
			NRF_TIMER1->EVENTS_COMPARE[2] = 0;
			NRF_TIMER1->EVENTS_COMPARE[3] = 0;
			// Interrupt setup
			NRF_TIMER1->INTENSET = (TIMER_INTENSET_COMPARE3_Enabled << TIMER_INTENSET_COMPARE3_Pos);
      attachInterrupt(TIMER1_IRQn, TIMER1_Interrupt);

			// Start clock
			NRF_TIMER1->TASKS_START = 1;

			turn_On_PPI_to_GPIO_for_PWM(ulPin, 0, NRF_TIMER1, 0);

			PWM_Channels_Value[0] = (2^PWM_RESOLUTION - 1) - mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
			Timer1_Compare_Unit_Occupied_by_Pin[0] = ulPin;
			Pin_Occupied_for_PWM[ulPin] = 1;
		}
		else
		{
			if (Timer1_Compare_Unit_Occupied_by_Pin[0] == 255)
			{
				// Configure ulPin as output
        digitalWrite(ulPin, LOW);
				pinMode(ulPin, OUTPUT);
				if (ulValue > 0)
				{
					// Configure GPIOTE channel "gpiote_channel" to toggle the PWM pin state
					// Note that we can only connect one GPIOTE task to an output pin
					nrf_gpiote_task_config(0, ulPin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
				}
				GPIOTE_Channels_Occupied[ulPin] = 0;
				turn_On_PPI_to_GPIO_for_PWM(ulPin, 0, NRF_TIMER1, 0);
				PWM_Channels_Value[0] = (2^PWM_RESOLUTION - 1) - mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
				Timer1_Compare_Unit_Occupied_by_Pin[0] = ulPin;
				Pin_Occupied_for_PWM[ulPin] = 1;
				NRF_TIMER1->EVENTS_COMPARE[0] = 0;
			}
			else if (Timer1_Compare_Unit_Occupied_by_Pin[1] == 255)
			{
				// Configure ulPin as output
        digitalWrite(ulPin, LOW);
				pinMode(ulPin, OUTPUT);
				if (ulValue > 0)
				{
					// Configure GPIOTE channel "gpiote_channel" to toggle the PWM pin state
					// Note that we can only connect one GPIOTE task to an output pin
					nrf_gpiote_task_config(1, ulPin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
				}
				GPIOTE_Channels_Occupied[ulPin] = 1;
				turn_On_PPI_to_GPIO_for_PWM(ulPin, 1, NRF_TIMER1, 1);
				PWM_Channels_Value[1] = (2^PWM_RESOLUTION - 1) - mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
				Timer1_Compare_Unit_Occupied_by_Pin[1] = ulPin;
				Pin_Occupied_for_PWM[ulPin] = 1;
				NRF_TIMER1->EVENTS_COMPARE[1] = 0;
			}
			else if (Timer1_Compare_Unit_Occupied_by_Pin[2] == 255)
			{
				// Configure ulPin as output
        digitalWrite(ulPin, LOW);
				pinMode(ulPin, OUTPUT);
				if (ulValue > 0)
				{
					// Configure GPIOTE channel "gpiote_channel" to toggle the PWM pin state
					// Note that we can only connect one GPIOTE task to an output pin
					nrf_gpiote_task_config(2, ulPin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
				}
				GPIOTE_Channels_Occupied[ulPin] = 2;
				turn_On_PPI_to_GPIO_for_PWM(ulPin, 2, NRF_TIMER1, 2);
				PWM_Channels_Value[2] = (2^PWM_RESOLUTION - 1) - mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
				Timer1_Compare_Unit_Occupied_by_Pin[2] = ulPin;
				Pin_Occupied_for_PWM[ulPin] = 1;
				NRF_TIMER1->EVENTS_COMPARE[2] = 0;
			}
			else
			{
				// All channels of Timer1 is occupied, need to use Timer2
				if (!RFduinoGZLL_used && Timer2_Compare_Unit_Occupied_by_Pin[0] == 255)
				{
					// Timer2 is not used: need to initialize it

					// Configure ulPin as output
          digitalWrite(ulPin, LOW);
					pinMode(ulPin, OUTPUT);

					if (ulValue > 0)
					{
						// Configure GPIOTE channel "gpiote_channel" to toggle the PWM pin state
						// Note that we can only connect one GPIOTE task to an output pin
						nrf_gpiote_task_config(3, ulPin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
					}
					GPIOTE_Channels_Occupied[ulPin] = 3;

					NRF_TIMER2->TASKS_STOP = 1;
					NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
					//NRF_TIMER2->PRESCALER = 6; // Source clock frequency is divided by 2^6 = 64
					NRF_TIMER2->PRESCALER = 0; // Source clock frequency is divided by 2^6 = 64 /////////////////////////////////////////
					//NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
					NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	////////////////////////////
					// Clears the timer, sets it to 0
					NRF_TIMER2->TASKS_CLEAR = 1;
					NRF_TIMER2->CC[0] = (2^PWM_RESOLUTION - 1);
					NRF_TIMER2->CC[3] = 0;
					NRF_TIMER2->EVENTS_COMPARE[0] = 0;
					NRF_TIMER2->EVENTS_COMPARE[3] = 0;

					// Interrupt setup
					NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE3_Enabled << TIMER_INTENSET_COMPARE3_Pos);
          attachInterrupt(TIMER2_IRQn, TIMER2_Interrupt);

					// Start clock
					NRF_TIMER2->TASKS_START = 1;

					turn_On_PPI_to_GPIO_for_PWM(ulPin, 3, NRF_TIMER2, 0);

					PWM_Channels_Value[3] = (2^PWM_RESOLUTION - 1) - mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
					Timer2_Compare_Unit_Occupied_by_Pin[0] = ulPin;
					Pin_Occupied_for_PWM[ulPin] = 1;
				}
				else
				{
					// Using all 4 TASK channels of GPIOTE, it is not possible to add another PWM channel.
				}
			}
		}
	}

/*
	// Defaults to digital write
	pinMode(ulPin, OUTPUT);
	ulValue = mapResolution(ulValue, _writeResolution, 8);
	if (ulValue < 128)
		digitalWrite(ulPin, LOW);
	else
		digitalWrite(ulPin, HIGH);
*/
}

#ifdef __cplusplus
}
#endif
