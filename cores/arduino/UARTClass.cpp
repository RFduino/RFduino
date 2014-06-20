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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "variant.h"
#include "UARTClass.h"

// Constructors

UARTClass::UARTClass( RingBuffer* pRx_buffer, RingBuffer* pTx_buffer )
{
  _rx_buffer = pRx_buffer ;
  _tx_buffer = pTx_buffer ;
}

// Public Methods

void UARTClass::begin( const uint32_t dwBaudRate )
{
	this->begin( dwBaudRate, UART_DEFAULT_RX_PIN, UART_DEFAULT_TX_PIN );
}

void UARTClass::begin( const uint32_t dwBaudRate, uint8_t rx_pin, uint8_t tx_pin )
{
  transmitting = false;
	UART0_Start( dwBaudRate, rx_pin, tx_pin );
}

void UARTClass::end( void )
{
  UART0_Stop();

  // clear any received data
  _rx_buffer->_iHead = _rx_buffer->_iTail ;

}

int UARTClass::available( void )
{
  return (uint32_t)(SERIAL_BUFFER_SIZE + _rx_buffer->_iHead - _rx_buffer->_iTail) % SERIAL_BUFFER_SIZE ;
}

int UARTClass::peek( void )
{
  if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
    return -1 ;

  return _rx_buffer->_aucBuffer[_rx_buffer->_iTail] ;
}

int UARTClass::read( void )
{
  // if the head isn't ahead of the tail, we don't have any characters
  if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
    return -1 ;

  uint8_t uc = _rx_buffer->_aucBuffer[_rx_buffer->_iTail] ;
  _rx_buffer->_iTail = (unsigned int)(_rx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE ;
  return uc ;
}

void UARTClass::flush( void )
{
  // Wait for any outstanding data to be sent
  while (transmitting)
    ;
}

void UARTClass::tx( void )
{
  if ( _tx_buffer->_iHead == _tx_buffer->_iTail )
  {
    transmitting = false;
    return;
  }

  transmitting = true;

  uint8_t uc = _tx_buffer->_aucBuffer[_tx_buffer->_iTail] ;
  _tx_buffer->_iTail = (unsigned int)(_tx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE ;

  UART0_TXD(uc);
}

size_t UARTClass::write( const uint8_t uc_data )
{
  // silently discard data if Serial.begin() hasn't been called
  if (UART0_State == UART0_State_NotStarted)
    return 1;

  // Wait if tx_buffer space is not available
  while ((_tx_buffer->_iHead + 1) % SERIAL_BUFFER_SIZE == _tx_buffer->_iTail)
    ;

  _tx_buffer->store_char(uc_data);

  if (! transmitting)
    tx();

  return 1;
}

void UARTClass::IrqHandler( void )
{
  if (UART0_TXReady())
  {
    UART0_TXReset();
    tx();
  }

  // did we receive data
  if (UART0_RXReady())
  {
    UART0_RXReset();

    uint8_t ch = UART0_RXData();

    if (UART0_RXErrorReset())
      return;

    _rx_buffer->store_char(ch);

    if (serialEvent)
      serialEvent();
  }
}
