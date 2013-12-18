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

#include "nrf51.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */
void Reset_Handler(void);
void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M0 core handlers */
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void POWER_CLOCK_IRQHandler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RADIO_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI0_TWI0_IRQHandler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI1_TWI1_IRQHandler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GPIOTE_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ADC_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TIMER0_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TIMER1_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TIMER2_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TEMP_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RNG_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ECB_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCM_AAR_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void QDEC_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WUCOMP_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SWI0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SWI1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SWI2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SWI3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SWI4_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SWI5_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {
    /* Configure Initial Stack Pointer, using linker-generated symbols */
    (void*) (&_estack),
    (void*) Reset_Handler,
    (void*) NMI_Handler,
    (void*) HardFault_Handler,
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) SVC_Handler,
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) PendSV_Handler,
    (void*) SysTick_Handler,

    /* Configurable interrupts */
    (void*) POWER_CLOCK_IRQHandler,    /* 0  Power Clock */
    (void*) RADIO_IRQHandler,          /* 1  Radio Controller*/
    (void*) UART0_IRQHandler,          /* 2  UART0 Controller */
    (void*) SPI0_TWI0_IRQHandler,      /* 3  SPI0 & TWI0 Controller*/
    (void*) SPI1_TWI1_IRQHandler,      /* 4  SPI1 & TWI1 Controller*/
    (void*) (0UL),                     /* 5  Reserved */
    (void*) GPIOTE_IRQHandler,         /* 6  GPIO */
    (void*) ADC_IRQHandler,            /* 7  ADC Controller */
    (void*) TIMER0_IRQHandler,         /* 8  Timer Counter 0 */
    (void*) TIMER1_IRQHandler,         /* 9  Timer Counter 1 */
    (void*) TIMER2_IRQHandler,         /* 10 Timer Counter 2 */
    (void*) RTC0_IRQHandler,           /* 11 Real Time Clock Controller 0 */
    (void*) TEMP_IRQHandler,           /* 12 Temperature */
    (void*) RNG_IRQHandler,            /* 13 Random Number Generator */
    (void*) ECB_IRQHandler,            /* 14 ECB */
    (void*) CCM_AAR_IRQHandler,        /* 15 CCM_AAR */
    (void*) WDT_IRQHandler,            /* 16 Watchdog Timer */
    (void*) RTC1_IRQHandler,           /* 17 Real Time Clock Controller 1 */
    (void*) QDEC_IRQHandler,           /* 18 QDEC */
    (void*) WUCOMP_IRQHandler,         /* 19 WUCOMP */
    (void*) SWI0_IRQHandler,           /* 20 Software Interrupt 0 */
    (void*) SWI1_IRQHandler,           /* 21 Software Interrupt 1 */
    (void*) SWI2_IRQHandler,           /* 22 Software Interrupt 2 */
    (void*) SWI3_IRQHandler,           /* 23 Software Interrupt 3 */
    (void*) SWI4_IRQHandler,           /* 24 Software Interrupt 4 */
    (void*) SWI5_IRQHandler,           /* 25 Software Interrupt 5 */
    (void*) (0UL),                     /* 26 Reserved */
    (void*) (0UL),                     /* 27 Reserved */
    (void*) (0UL),                     /* 28 Reserved */
    (void*) (0UL),                     /* 29 Reserved */
    (void*) (0UL),                     /* 30 Reserved */
    (void*) (0UL)                      /* 31 Reserved */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;

    /* Initialize the relocate segment */
    pSrc = &_etext;
    pDest = &_srelocate;

    if (pSrc != pDest) {
        for (; pDest < &_erelocate;) {
            *pDest++ = *pSrc++;
        }
    }

    /* Clear the zero segment */
    for (pDest = &_szero; pDest < &_ezero;) {
        *pDest++ = 0;
    }

    /* Initialize the C library */
    __libc_init_array();

    /* Call SystemInit */
    SystemInit();

    /* Branch to main function */
    main();

    /* Infinite loop */
    while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
    while (1) {
    }
}
