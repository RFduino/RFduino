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

#include "variant.h"

#define LFCLK_FREQUENCY    (32768UL)

/*
 * UART objects
 */

RingBuffer rxBuffer, txBuffer;

UARTClass Serial( &rxBuffer, &txBuffer );

// IT handlers
void UART0_Interrupt()
{
  Serial.IrqHandler();
}

#ifdef __cplusplus
extern "C" {
#endif

bool override_uart_limit = false;

/*
 * Pins descriptions
 */
extern uint8_t PPI_Channels_Occupied[PINS_COUNT][2] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
extern uint8_t GPIOTE_Channels_Occupied[PINS_COUNT] = {255, 255, 255, 255, 255, 255, 255};
extern uint8_t Timer1_Compare_Unit_Occupied_by_Pin[3] = {255, 255, 255}; // Contains Pin number which occupied appropriate CC register (0, 1 or 2) of Timer 1 for PWM purposes
extern uint8_t Timer2_Compare_Unit_Occupied_by_Pin[3] = {255, 255, 255}; // Contains Pin number which occupied appropriate CC register (0, 1 or 2) of Timer 2 for PWM purposes
extern uint8_t Pin_Occupied_for_PWM[PINS_COUNT] = {0, 0, 0, 0, 0, 0, 0}; // Determines if particular pin is occupied for PWM output

// declared in wiring.c
extern "C" void RTC1_Interrupt(void);

void rtc_config()
{
    NRF_RTC1->TASKS_STOP = 1;	// Stop RTC timer
    NRF_RTC1->TASKS_CLEAR = 1;	// Clear timer
	  NRF_RTC1->PRESCALER = 0;	// No prescaling => 1 tick = 1/32768Hz = 30.517us
	  NRF_RTC1->EVTENSET = (RTC_EVTENSET_OVRFLW_Set << RTC_EVTENSET_OVRFLW_Pos); // Enable OVRFLW Event
	  NRF_RTC1->INTENSET = (RTC_INTENSET_OVRFLW_Set << RTC_INTENSET_OVRFLW_Pos); // Enable OVRFLW Interrupt
    attachInterrupt(RTC1_IRQn, RTC1_Interrupt);
    NRF_RTC1->TASKS_START = 1;	// Start RTC
}

void init( void )
{
    // NRF51822 doesn't implement SysTick, so use the RTC for timing
    rtc_config();
}

/*
 * UART0 core
 */

// UART0_TX is called by syscalls _write (note: printf() must end with '\n')
// we don't want to lockup waiting for TX until after UART0_Start()

// NRF_UART0->EVENTS_TXDRDY is initialized to 0 at power up
// we cannot test this event until after one byte has been transmitted

UART0_States UART0_State = UART0_State_NotStarted;

int UART0_BaudRate()
{
  switch ((NRF_UART0->BAUDRATE & UART_BAUDRATE_BAUDRATE_Msk) >> UART_BAUDRATE_BAUDRATE_Pos)
  {
    case UART_BAUDRATE_BAUDRATE_Baud1200: return 1200;
    case UART_BAUDRATE_BAUDRATE_Baud2400: return 2400;
    case UART_BAUDRATE_BAUDRATE_Baud4800: return 4800;
    case UART_BAUDRATE_BAUDRATE_Baud9600: return 9600;
    case UART_BAUDRATE_BAUDRATE_Baud14400: return 14400;
    case UART_BAUDRATE_BAUDRATE_Baud19200: return 19200;
    case UART_BAUDRATE_BAUDRATE_Baud28800: return 28800;
    case UART_BAUDRATE_BAUDRATE_Baud31250: return 31250;
    case UART_BAUDRATE_BAUDRATE_Baud38400: return 38400;
    case UART_BAUDRATE_BAUDRATE_Baud57600: return 57600;
    case UART_BAUDRATE_BAUDRATE_Baud76800: return 76800;
    case UART_BAUDRATE_BAUDRATE_Baud115200: return 115200;
    case UART_BAUDRATE_BAUDRATE_Baud230400: return 230400;
    case UART_BAUDRATE_BAUDRATE_Baud250000: return 250000;
    case UART_BAUDRATE_BAUDRATE_Baud460800: return 460800;
    case UART_BAUDRATE_BAUDRATE_Baud921600: return 921600;
    case UART_BAUDRATE_BAUDRATE_Baud1M: return 1000000;
  }
  return 0;
}

void UART0_Start( int dwBaudRate, uint8_t rx_pin, uint8_t tx_pin )
{
  // must STOP before restarting
  if (UART0_State != UART0_State_NotStarted)
    return;

  NRF_GPIO->PIN_CNF[tx_pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
              | (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos)
              | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
              | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
              | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);

  NRF_GPIO->PIN_CNF[rx_pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
              | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
              | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
              | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
              | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

  NRF_UART0->PSELTXD = tx_pin;
  NRF_UART0->PSELRXD = rx_pin;

  uint32_t dw;
  switch (dwBaudRate)
  {
    case 1200: dw = UART_BAUDRATE_BAUDRATE_Baud1200; break;
    case 2400: dw = UART_BAUDRATE_BAUDRATE_Baud2400; break;
    case 4800: dw = UART_BAUDRATE_BAUDRATE_Baud4800; break;
    case 9600: dw = UART_BAUDRATE_BAUDRATE_Baud9600; break;
    case 14400: dw = UART_BAUDRATE_BAUDRATE_Baud14400; break;
    case 19200: dw = UART_BAUDRATE_BAUDRATE_Baud19200; break;
    case 28800: dw = UART_BAUDRATE_BAUDRATE_Baud28800; break;
    case 31250: dw = UART_BAUDRATE_BAUDRATE_Baud31250; break;
    case 38400: dw = UART_BAUDRATE_BAUDRATE_Baud38400; break;
    case 57600: dw = UART_BAUDRATE_BAUDRATE_Baud57600; break;
    case 76800: dw = UART_BAUDRATE_BAUDRATE_Baud76800; break;
    case 115200: dw = UART_BAUDRATE_BAUDRATE_Baud115200; break;
    case 230400: dw = UART_BAUDRATE_BAUDRATE_Baud230400; break;
    case 250000: dw = UART_BAUDRATE_BAUDRATE_Baud250000; break;
    case 460800: dw = UART_BAUDRATE_BAUDRATE_Baud460800; break;
    case 921600: dw = UART_BAUDRATE_BAUDRATE_Baud921600; break;
    case 1000000: dw = UART_BAUDRATE_BAUDRATE_Baud1M; break;
  }

  NRF_UART0->BAUDRATE         = (dw << UART_BAUDRATE_BAUDRATE_Pos);
  NRF_UART0->ENABLE           = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
  NRF_UART0->EVENTS_RXDRDY    = 0;
  NRF_UART0->EVENTS_TXDRDY    = 0;

  NRF_UART0->INTENSET        |= (UART_INTENSET_RXDRDY_Enabled << UART_INTENSET_RXDRDY_Pos )
                              | (UART_INTENSET_TXDRDY_Enabled << UART_INTENSET_TXDRDY_Pos );

  UART0_State = UART0_State_BeforeFirstTX;

  attachInterrupt(UART0_IRQn, UART0_Interrupt);

  NRF_UART0->TASKS_STARTTX    = 1;
  NRF_UART0->TASKS_STARTRX    = 1;

  if (! override_uart_limit)
  {
    if (RFduinoBLE_enabled && dwBaudRate > 9600)
    {
      const char *error = "BLE + UART > 9600 baud not recommended due to critical BLE timing requirements.\r\n"
        "To override, add: override_uart_limit = true; to the top of setup() in your sketch.";

      // attempt to notify user of error condition
      const char *p = error;
      while (*p)
        UART0_TX(*p++);

      // don't continue
      while (1)
        ;
    }
  }
}

void UART0_Stop()
{
  if (UART0_State == UART0_State_NotStarted)
    return;

  // Wait for any outstanding data to be sent
  Serial.flush();

  NRF_UART0->TASKS_STOPTX = 1;
  NRF_UART0->TASKS_STOPRX = 1;

  NRF_UART0->ENABLE       = (UART_ENABLE_ENABLE_Disabled << UART_ENABLE_ENABLE_Pos);

  // Disable UART interrupt in NVIC
  detachInterrupt(UART0_IRQn);

  // Disconnect high drive tx pin
  int tx_pin = NRF_UART0->PSELTXD;
  NRF_GPIO->PIN_CNF[tx_pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
              | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
              | (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
              | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
              | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);                    

  UART0_State = UART0_State_NotStarted;
}

void UART0_FlushTX()
{
  Serial.flush(); 
}

// delegate to serial for syscalls/write and error messages
void UART0_TX( const uint8_t uc_data )
{
  Serial.write( uc_data );
}

void UART0_TXD( const uint8_t uc_data )
{
  // tx byte
  NRF_UART0->TXD = uc_data;

  // don't change start if not started
  if (UART0_State == UART0_State_BeforeFirstTX)
    UART0_State = UART0_State_AfterFirstTX;
}

// UART0_RXReady declared inline in variant.h

// UART0_RXData declared inline in variant.h

void UART0_RXReset()
{
  NRF_UART0->EVENTS_RXDRDY = 0;
}

void UART0_TXReset()
{
  NRF_UART0->EVENTS_TXDRDY = 0;
}

int UART0_RXErrorReset()
{
  if (NRF_UART0->ERRORSRC & UART_ERRORSRC_OVERRUN_Msk)
  {
    NRF_UART0->ERRORSRC = (UART_ERRORSRC_OVERRUN_Clear << UART_ERRORSRC_OVERRUN_Pos);
    return true;
  }
  /*
  else if (NRF_UART0->ERRORSRC & UART_ERRORSRC_PARITY_Msk)
  {
    NRF_UART0->ERRORSRC = (UART_ERRORSRC_PARITY_Clear << UART_ERRORSRC_PARITY_Pos);
  }
  */
  else if (NRF_UART0->ERRORSRC & UART_ERRORSRC_FRAMING_Msk)
  {
    NRF_UART0->ERRORSRC = (UART_ERRORSRC_FRAMING_Clear << UART_ERRORSRC_FRAMING_Pos);
    return true;
  }
  /*
  else if (NRF_UART0->ERRORSRC & UART_ERRORSRC_BREAK_Msk)
  {
    NRF_UART0->ERRORSRC = (UART_ERRORSRC_BREAK_Clear << UART_ERRORSRC_BREAK_Pos);
  }
  */

  return false;
}

uint8_t UART0_RX()
{
  uint8_t uc_data;

  // if you call UART0_RX(), you expect UART0 to be Started (no need to check UART0_State)

  do
  {
    // byte available
    while (! UART0_RXReady())
      ;

    UART0_RXReset();

    uc_data = UART0_RXData();

  } while (UART0_RXErrorReset());

  return uc_data;
}

uint32_t getDeviceIdLow()
{
  return NRF_FICR->DEVICEID[0];
}

uint32_t getDeviceIdHigh()
{
  return NRF_FICR->DEVICEID[1];
}

uint64_t getDeviceId()
{
  return ((uint64_t)getDeviceIdHigh() << 32) | getDeviceIdLow();
}

#ifdef __cplusplus
}
#endif
