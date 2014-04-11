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
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"

SPIClass::SPIClass(NRF_SPI_Type *_spi, int _pinSCK, int _pinMOSI, int _pinMISO) : spi(_spi)
{
    pinSCK = _pinSCK;
    pinMOSI = _pinMOSI;
    pinMISO = _pinMISO;
}

void SPIClass::begin()
{
    pinMode(pinSCK, OUTPUT);
    pinMode(pinMOSI, OUTPUT);
    pinMode(pinMISO, INPUT);

    spi->PSELSCK = pinSCK;
    spi->PSELMOSI = pinMOSI;
    spi->PSELMISO = pinMISO;

	// Default speed set to 4Mhz
	setFrequency(4000);
	setDataMode(SPI_MODE0);
	setBitOrder(MSBFIRST);

    spi->EVENTS_READY = 0;

	spi->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}

void SPIClass::end()
{
	spi->ENABLE &= ~(SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}

void SPIClass::setBitOrder(BitOrder _bitOrder)
{
	if (_bitOrder == MSBFIRST)
		spi->CONFIG &= ~(SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos);
	else
    spi->CONFIG |= (SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos);
}

void SPIClass::setDataMode(uint8_t _dataMode)
{
  setCPOL((_dataMode & 0x02) >> 1);
  setCPHA(_dataMode & 0x01);
}

void SPIClass::setCPOL(bool _activeLow)
{
	if (_activeLow)
	  spi->CONFIG |= (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
	else
	  spi->CONFIG &= ~(SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
}

void SPIClass::setCPHA(bool _trailing)
{
	if (_trailing)
	  spi->CONFIG |= (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos);
	else
	  spi->CONFIG &= ~(SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos);
}

void SPIClass::setFrequency(int _speedKbps)
{
    if (_speedKbps == 125)
    	spi->FREQUENCY = (SPI_FREQUENCY_FREQUENCY_K125 << SPI_FREQUENCY_FREQUENCY_Pos);
    else if (_speedKbps == 250)
    	spi->FREQUENCY = (SPI_FREQUENCY_FREQUENCY_K250 << SPI_FREQUENCY_FREQUENCY_Pos);
    else if (_speedKbps == 500)
    	spi->FREQUENCY = (SPI_FREQUENCY_FREQUENCY_K500 << SPI_FREQUENCY_FREQUENCY_Pos);
    else if (_speedKbps == 1000)
    	spi->FREQUENCY = (SPI_FREQUENCY_FREQUENCY_M1 << SPI_FREQUENCY_FREQUENCY_Pos);
    else if (_speedKbps == 2000)
    	spi->FREQUENCY = (SPI_FREQUENCY_FREQUENCY_M2 << SPI_FREQUENCY_FREQUENCY_Pos);
    else if (_speedKbps == 4000)
    	spi->FREQUENCY = (SPI_FREQUENCY_FREQUENCY_M4 << SPI_FREQUENCY_FREQUENCY_Pos);
    else if (_speedKbps == 8000)
    	spi->FREQUENCY = (SPI_FREQUENCY_FREQUENCY_M8 << SPI_FREQUENCY_FREQUENCY_Pos);
}

void SPIClass::setClockDivider(uint8_t _divider)
{
}

byte SPIClass::transfer(uint8_t _data)
{
    spi->TXD = _data;

    while (spi->EVENTS_READY == 0)
      ;
    spi->EVENTS_READY = 0;

    _data = spi->RXD;

    return _data;
}

void SPIClass::attachInterrupt(void)
{
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void)
{
	// Should be disableInterrupt()
}

#if SPI_INTERFACES_COUNT > 0
SPIClass SPI(SPI_INTERFACE, PIN_SPI_SCK, PIN_SPI_MOSI, PIN_SPI_MISO);
#endif
