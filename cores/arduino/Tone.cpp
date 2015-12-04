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

/* Tone.cpp

  A Tone Generator Library

 */

#include "Tone.h"

static uint32_t tone_toggle_count;
static uint8_t tone_pin;

static void TIMER2_Interrupt(void);

void tone(uint8_t ulPin, uint16_t frequency, uint32_t duration)
{
	uint32_t i, prescaler, compare;
	if (ulPin >= PINS_COUNT) return;
	// Stops PWM if enabled
	if (Pin_Occupied_for_PWM[ulPin])
	{
		// Disable GPIOTE and PPI channels from GPIO
		turn_Off_GPIOTE_PPI_from_GPIO(ulPin);
		Pin_Occupied_for_PWM[ulPin] = 0;
	}

	tone_pin = ulPin;

	// Find appropriate values for PRESCALER and COMPARE registers
	for (i = 0; i <= 9; i++)
	{
		prescaler = i;
		compare = VARIANT_MCK / frequency;
		compare = compare >> (prescaler + 1);
		compare = compare - 1;
		if ((compare >= 2) && (compare <= 65535))
			break;
	}
	// Calculate interrupts count
	if (duration)
	{
		tone_toggle_count = ((frequency * duration) / 1000) * 2;
	}
	else
	{
		tone_toggle_count = 0xFFFFFFFF;
	}

	// Configure ulPin as output
	digitalWrite(ulPin, LOW);
	pinMode(ulPin, OUTPUT);

	// Configure GPIOTE channel "3" to toggle the PWM pin state
	// Note that we can only connect one GPIOTE task to an output pin
	nrf_gpiote_task_config(3, ulPin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
	GPIOTE_Channels_Occupied[ulPin] = 3;

	// Configure PPI channel "6" for toggling pin
  rfduino_ppi_channel_assign(6, &NRF_TIMER2->EVENTS_COMPARE[0], &NRF_GPIOTE->TASKS_OUT[3]);

	// Configure Timer 2
	NRF_TIMER2->TASKS_STOP = 1;
	NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER2->PRESCALER = prescaler; // Source clock frequency is divided by 2^prescaler
	NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
	// Enable Shortcut between CC[0] event and the CLEAR task
	NRF_TIMER2->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
	// Clears the timer, sets it to 0
	NRF_TIMER2->TASKS_CLEAR = 1;
	NRF_TIMER2->CC[0] = (uint16_t)(compare);
	NRF_TIMER2->EVENTS_COMPARE[0] = 0;
	// Interrupt setup
	NRF_TIMER2->INTENCLR = 0xFFFFFFFF;
	NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
	attachInterrupt(TIMER2_IRQn, TIMER2_Interrupt);
	// Start clock
	NRF_TIMER2->TASKS_START = 1;
}

void tone(uint8_t ulPin, uint16_t frequency)
{
	tone(ulPin, frequency, 0);
}

static void TIMER2_Interrupt(void)
{
	NRF_TIMER2->EVENTS_COMPARE[0] = 0;

	if (tone_toggle_count)
	{
		tone_toggle_count--;
	}
	else
	{
		// Stop Timer 2
		NRF_TIMER2->TASKS_STOP = 1;
		// Unconfig GPIOTE channel 3
		nrf_gpiote_unconfig(3);
		GPIOTE_Channels_Occupied[tone_pin] = 255;
		// Unconfig PPI channel 6
		rfduino_ppi_channel_unassign(6);
		digitalWrite(tone_pin, LOW);
		tone_pin = 255;
	}
}

void noTone(uint8_t ulPin)
{
	if (ulPin >= PINS_COUNT) return;
	if (ulPin == tone_pin) {
		// Stop Timer 2
		NRF_TIMER2->TASKS_STOP = 1;
		// Unconfig GPIOTE channel 3
		nrf_gpiote_unconfig(3);
		GPIOTE_Channels_Occupied[tone_pin] = 255;
		// Unconfig PPI channel 6
		rfduino_ppi_channel_unassign(6);
		digitalWrite(tone_pin, LOW);
		tone_pin = 255;
	}
}
