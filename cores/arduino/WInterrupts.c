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
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#include "WInterrupts.h"

static callback_t callbacks[32];

// -15 Reset_Handler
// -14 NMI_Handler
// -13 HardFault_Handler
// -12 reserved
// -11 reserved
// -10 reserved
// -9 reserved
// -8 reserved
// -7 reserved
// -6 reserved
// -5 SVC_Handler
// -4 DebugMonitor_Handler
// -3 reserved
// -2 PendSV_Handler
// -1 SysTick_Handler

// configurable interrupts
void POWER_CLOCK_IRQHandler() { callbacks[POWER_CLOCK_IRQn](); }  // 0 Power Clock
void RADIO_IRQHandler()       { callbacks[RADIO_IRQn](); }        // 1 Radio
void UART0_IRQHandler()       { callbacks[UART0_IRQn](); }        // 2 UART 0
void SPI0_TWI0_IRQHandler()   { callbacks[SPI0_TWI0_IRQn](); }    // 3 SPI0 and TWI0
void SPI1_TWI1_IRQHandler()   { callbacks[SPI1_TWI1_IRQn](); }    // 4 SPI1 and TWI1
// reserved                                                       // 5 reserved
void GPIOTE_IRQHandler()      { callbacks[GPIOTE_IRQn](); }       // 6 General Purpose IO Tasks and Events
void ADC_IRQHandler()         { callbacks[ADC_IRQn](); }          // 7 Analog to Digital Converter
void TIMER0_IRQHandler()      { callbacks[TIMER0_IRQn](); }       // 8 Timer 0
void TIMER1_IRQHandler()      { callbacks[TIMER1_IRQn](); }       // 9 Timer 1
void TIMER2_IRQHandler()      { callbacks[TIMER2_IRQn](); }       // 10 Timer 2
void RTC0_IRQHandler()        { callbacks[RTC0_IRQn](); }         // 11 Real Time Clock 0
void TEMP_IRQHandler()        { callbacks[TEMP_IRQn](); }         // 12 Temperature
void RNG_IRQHandler()         { callbacks[RNG_IRQn](); }          // 13 Random Number Generator
void ECB_IRQHandler()         { callbacks[ECB_IRQn](); }          // 14 ECB
void CCM_AAR_IRQHandler()     { callbacks[CCM_AAR_IRQn](); }      // 15 CCM AAR
void WDT_IRQHandler()         { callbacks[WDT_IRQn](); }          // 16 Watchdog Timer
void RTC1_IRQHandler()        { callbacks[RTC1_IRQn](); }         // 17 Real Time Clock 1
void QDEC_IRQHandler()        { callbacks[QDEC_IRQn](); }         // 18 QDEC
void WUCOMP_IRQHandler()      { callbacks[WUCOMP_COMP_IRQn](); }  // 19 WUCOMP
//the blestack uses these (multiply defined)
//void SWI0_IRQHandler()        { callbacks[SWI0_IRQn](); }         // 20 Software Interrupt 0
//void SWI1_IRQHandler()        { callbacks[SWI1_IRQn](); }         // 21 Software Interrupt 0
//void SWI2_IRQHandler()        { callbacks[SWI2_IRQn](); }         // 22 Software Interrupt 0
//void SWI3_IRQHandler()        { callbacks[SWI3_IRQn](); }         // 23 Software Interrupt 0
//void SWI4_IRQHandler()        { callbacks[SWI4_IRQn](); }         // 24 Software Interrupt 0
//void SWI5_IRQHandler()        { callbacks[SWI5_IRQn](); }         // 25 Software Interrupt 0
// reserved                                                       // 26 reserved
// reserved                                                       // 27 reserved
// reserved                                                       // 28 reserved
// reserved                                                       // 29 reserved
// reserved                                                       // 30 reserved
// reserved                                                       // 31 reserved

void attachInterrupt(uint8_t IRQn, callback_t callback)
{
  callbacks[IRQn] = callback;
  rfduino_nvic_enableirq(IRQn);
}

void detachInterrupt(uint8_t IRQn)
{
  rfduino_nvic_disableirq(IRQn);
  callbacks[IRQn] = NULL;
}

void attachPinInterrupt(uint32_t pin, pin_callback_t callback, uint32_t mode)
{
  RFduino_pinWakeCallback(pin, mode, callback);
}

void detachPinInterrupt(uint32_t pin)
{
  RFduino_pinWakeCallback(pin, DISABLE, NULL);
}
