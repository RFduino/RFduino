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
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@bug.st>.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

extern "C" {
#include <string.h>
}

#include "Wire.h"
/*
static inline bool TWI_FailedAcknowledge(NRF_TWI_Type *pTwi) {
	return pTwi->ERRORSRC & (TWI_ERRORSRC_DNACK_Msk | TWI_ERRORSRC_ANACK_Msk);
}

static inline bool TWI_WaitTransferComplete(NRF_TWI_Type *_twi, uint32_t _timeout) {
	while (!TWI_TransferComplete(_twi)) {
		if (TWI_FailedAcknowledge(_twi))
			return false;
		if (--_timeout == 0)
			return false;
	}
	return true;
}

static inline bool TWI_WaitByteSent(NRF_TWI_Type *_twi, uint32_t _timeout) {
	while (!TWI_ByteSent(_twi)) {
		if (TWI_FailedAcknowledge(_twi))
			return false;
		if (--_timeout == 0)
			return false;
	}
	return true;
}

static inline bool TWI_WaitByteReceived(NRF_TWI_Type *_twi, uint32_t _timeout) {
	while (!TWI_ByteReceived(_twi)) {
		if (TWI_FailedAcknowledge(_twi))
			return false;
		if (--_timeout == 0)
			return false;
	}
	return true;
}*/

// static inline bool TWI_STATUS_SVREAD(uint32_t status) {
	// return (status & TWI_SR_SVREAD) == TWI_SR_SVREAD;
// }

// static inline bool TWI_STATUS_SVACC(uint32_t status) {
	// return (status & TWI_SR_SVACC) == TWI_SR_SVACC;
// }

// static inline bool TWI_STATUS_GACC(uint32_t status) {
	// return (status & TWI_SR_GACC) == TWI_SR_GACC;
// }

// static inline bool TWI_STATUS_EOSACC(uint32_t status) {
	// return (status & TWI_SR_EOSACC) == TWI_SR_EOSACC;
// }

static inline bool TWI_STATUS_ANACK(uint32_t status) {
	return (status & TWI_ERRORSRC_ANACK_Msk) == TWI_ERRORSRC_ANACK_Msk;
}

static inline bool TWI_STATUS_DNACK(uint32_t status) {
	return (status & TWI_ERRORSRC_DNACK_Msk) == TWI_ERRORSRC_DNACK_Msk;
}

TwoWire::TwoWire(NRF_TWI_Type *_twi) :
	twi(_twi), rxBufferIndex(0), rxBufferLength(0), txAddress(0),
			txBufferLength(0), srvBufferIndex(0), srvBufferLength(0),
      status(UNINITIALIZED), speed(100), PPI_channel(255)
{
	// Empty
}

/**
 * Detects stuck slaves (SDA = 0 and SCL = 1) and tries to clear the bus.
 *
 * @return
 * @retval false Bus is stuck.
 * @retval true Bus is clear.
 */
bool TwoWire::twi_master_clear_bus(void)
{
	uint32_t twi_state;
    bool bus_clear;

	// Save and disable TWI hardware so software can take control over the pins.
    twi_state = twi->ENABLE;
    twi->ENABLE = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos;

    NRF_GPIO->PIN_CNF[SCL_pin_number] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
      | (GPIO_PIN_CNF_DRIVE_H0D1     << GPIO_PIN_CNF_DRIVE_Pos)
      | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)
      | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
      | (GPIO_PIN_CNF_DIR_Output     << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[SDA_pin_number] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
      | (GPIO_PIN_CNF_DRIVE_H0D1     << GPIO_PIN_CNF_DRIVE_Pos)
      | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)
      | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
      | (GPIO_PIN_CNF_DIR_Output     << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->OUTSET = (1UL << SDA_pin_number);
  	NRF_GPIO->OUTSET = (1UL << SCL_pin_number);
  	TWI_DELAY();

    if (((NRF_GPIO->IN >> SDA_pin_number) & 0x1UL) && ((NRF_GPIO->IN >> SCL_pin_number) & 0x1UL))
    {
        bus_clear = true;
    }
    else
    {
        uint_fast8_t i;
        bus_clear = false;

        // Clock max 18 pulses worst case scenario(9 for master to send the rest of command and 9 for slave to respond) to SCL line and wait for SDA come high
        for (i=18; i--;)
        {
            NRF_GPIO->OUTCLR = (1UL << SCL_pin_number);
			TWI_DELAY();
			NRF_GPIO->OUTSET = (1UL << SCL_pin_number);
			TWI_DELAY();

            if ((NRF_GPIO->IN >> SDA_pin_number) & 0x1UL)
            {
                bus_clear = true;
                break;
            }
        }
    }

	twi->ENABLE = twi_state;

    return bus_clear;
}

// TWI module lock-up (PAN 56)
void TwoWire::twi_peripheral_recovery(void)
{
    twi->ENABLE = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos;

    *(uint32_t *)((uint32_t)twi + 0xFFC) = 0;

    delayMicroseconds(5);

    *(uint32_t *)((uint32_t)twi + 0xFFC) = 1;

    twi->ENABLE = TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos;

    twi_master_init();
}

bool TwoWire::twi_master_init(void)
{
    /* To secure correct signal levels on the pins used by the TWI
       master when the system is in OFF mode, and when the TWI master is
       disabled, these pins must be configured in the GPIO peripheral.
    */
    NRF_GPIO->PIN_CNF[SCL_pin_number] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
      | (GPIO_PIN_CNF_DRIVE_H0D1     << GPIO_PIN_CNF_DRIVE_Pos)
      | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)
      | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
      | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->PIN_CNF[SDA_pin_number] =
        (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
      | (GPIO_PIN_CNF_DRIVE_H0D1     << GPIO_PIN_CNF_DRIVE_Pos)
      | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)
      | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
      | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos);

    twi->EVENTS_RXDREADY = 0;
    twi->EVENTS_TXDSENT = 0;
    twi->PSELSCL = SCL_pin_number;
    twi->PSELSDA = SDA_pin_number;

    uint32_t freq TWI_FREQUENCY_FREQUENCY_K100;;
    if (speed == 250)
      freq = TWI_FREQUENCY_FREQUENCY_K250;
    else if (speed == 400)
      freq = TWI_FREQUENCY_FREQUENCY_K400;
    twi->FREQUENCY = freq;

    if (PPI_channel == 255)
    {
      PPI_channel = find_free_PPI_channel(255);
      if (PPI_channel == 255)
        Serial.println("no free PPI channels for twi");
    }

    // reserve the PPI channel
    rfduino_ppi_channel_assign(PPI_channel, 0, 0);

    twi->ENABLE = TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos;

	twi_master_clear_bus();

    return true;
}

uint8_t TwoWire::twi_master_read(uint8_t *data, uint8_t data_length, uint8_t issue_stop_condition)
{
    uint32_t timeout = MAX_TIMEOUT_LOOPS;   /* max loops to wait for RXDREADY event*/
	uint8_t bytes_received = 0;

    if(data_length == 0)
    {
        /* gently return false for requesting data of size 0 */
        return 0;
    }

    if ((data_length == 1) && (issue_stop_condition == 1))
    {
        rfduino_ppi_channel_assign(PPI_channel, &twi->EVENTS_BB, &twi->TASKS_STOP);
    }
    else
    {
        rfduino_ppi_channel_assign(PPI_channel, &twi->EVENTS_BB, &twi->TASKS_SUSPEND);
    }
    twi->TASKS_STARTRX = 1;
    while(true)
    {
        while((twi->EVENTS_RXDREADY == 0) && (--timeout))
        {
        }

        if(timeout == 0)
        {
            // unassign the PPI channel (but keep it reserved)
            rfduino_ppi_channel_assign(PPI_channel, 0, 0);

            // NACK received after sending the address
            if (twi->ERRORSRC & TWI_ERRORSRC_ANACK_Msk)
            {
              twi->ERRORSRC |= TWI_ERRORSRC_ANACK_Clear << TWI_ERRORSRC_ANACK_Pos;

              twi->EVENTS_STOPPED = 0;
              twi->TASKS_STOP = 1;
              /* wait until stop sequence is sent and clear the EVENTS_STOPPED */
              while (twi->EVENTS_STOPPED == 0)
              {
              }

              return 0;
            }

            // NACK received after sending a data-byte
            if (twi->ERRORSRC & TWI_ERRORSRC_DNACK_Msk)
            {
              twi->ERRORSRC |= TWI_ERRORSRC_DNACK_Clear << TWI_ERRORSRC_DNACK_Pos;

              twi->EVENTS_STOPPED = 0;
              twi->TASKS_STOP = 1;
              /* wait until stop sequence is sent and clear the EVENTS_STOPPED */
              while (twi->EVENTS_STOPPED == 0)
              {
              }

              return 0;
            }

            // TWI module lock-up (PAN 56)
            twi_peripheral_recovery();

            return 0;
        }

        twi->EVENTS_RXDREADY = 0;
        *data++ = twi->RXD;
		bytes_received++;

        /* configure PPI to stop TWI master before we get last BB event */
        if (--data_length == 1)
        {
            if (issue_stop_condition == 1)
			{
        rfduino_ppi_channel_assign(PPI_channel, &twi->EVENTS_BB, &twi->TASKS_STOP);
			}
        }

        if (data_length == 0)
            break;

        // TWI module lock-up (PAN 56)
        delayMicroseconds(20);

        twi->TASKS_RESUME = 1;
    }

    if (issue_stop_condition == 1)
	{
		/* wait until stop sequence is sent and clear the EVENTS_STOPPED */
		while(twi->EVENTS_STOPPED == 0)
		{
		}
		twi->EVENTS_STOPPED = 0;
	}

    // unassign the PPI channel (but keep it reserved)
    rfduino_ppi_channel_assign(PPI_channel, 0, 0);
    return bytes_received;
}

uint8_t TwoWire::twi_master_write(uint8_t *data, uint8_t data_length, uint8_t issue_stop_condition)
{
    uint32_t timeout = MAX_TIMEOUT_LOOPS;   /* max loops to wait for EVENTS_TXDSENT event*/

    if (data_length == 0)
    {
        /* gently return false for requesting data of size 0 */
        return 4;
    }

    twi->TXD = *data++;
    twi->TASKS_STARTTX = 1;

    while (true)
    {
        while((twi->EVENTS_TXDSENT == 0) && (--timeout))
        {
        }

        if (timeout == 0)
        {
            // NACK received after sending the address
            if (twi->ERRORSRC & TWI_ERRORSRC_ANACK_Msk)
            {
              twi->ERRORSRC |= TWI_ERRORSRC_ANACK_Clear << TWI_ERRORSRC_ANACK_Pos;

              twi->EVENTS_STOPPED = 0;
              twi->TASKS_STOP = 1;
              /* wait until stop sequence is sent and clear the EVENTS_STOPPED */
              while (twi->EVENTS_STOPPED == 0)
              {
              }

              return 2;
            }

            // NACK received after sending a data-byte
            if (twi->ERRORSRC & TWI_ERRORSRC_DNACK_Msk)
            {
              twi->ERRORSRC |= TWI_ERRORSRC_DNACK_Clear << TWI_ERRORSRC_DNACK_Pos;

              twi->EVENTS_STOPPED = 0;
              twi->TASKS_STOP = 1;
              /* wait until stop sequence is sent and clear the EVENTS_STOPPED */
              while (twi->EVENTS_STOPPED == 0)
              {
              }

              return 3;
            }

            // TWI module lock-up (PAN 56)
            twi_peripheral_recovery();

            return 4;
        }

        twi->EVENTS_TXDSENT = 0;
        if (--data_length == 0)
        {
            break;
        }

        twi->TXD = *data++;
    }

    if (issue_stop_condition == 1)
    {
        twi->EVENTS_STOPPED = 0;
        twi->TASKS_STOP = 1;
        /* wait until stop sequence is sent and clear the EVENTS_STOPPED */
        while(twi->EVENTS_STOPPED == 0)
        {
        }
    }

    return 0;
}

void TwoWire::begin(void) {
	beginOnPins(PIN_WIRE_SCL, PIN_WIRE_SDA);
}

void TwoWire::beginOnPins(uint8_t clock_pin, uint8_t data_pin)
{
	//if (onBeginCallback)
	//	onBeginCallback();

	SCL_pin_number = clock_pin;
	SDA_pin_number = data_pin;
	twi_master_init();
	status = MASTER_IDLE;
}

/*
void TwoWire::begin(uint8_t address) {
	if (onBeginCallback)
		onBeginCallback();

	// Disable PDC channel
	twi->TWI_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	TWI_ConfigureSlave(twi, address);
	status = SLAVE_IDLE;
	TWI_EnableIt(twi, TWI_IER_SVACC);
	//| TWI_IER_RXRDY | TWI_IER_TXRDY	| TWI_IER_TXCOMP);
}

void TwoWire::begin(int address) {
	begin((uint8_t) address);
}*/

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
	// perform blocking read into buffer
	uint8_t readed = 0;
	// clamp to buffer length
	if(quantity > BUFFER_LENGTH)
		quantity = BUFFER_LENGTH;

	if (quantity > 0)
    {
        twi->ADDRESS = address;
        readed = twi_master_read(rxBuffer, quantity, sendStop);
    }

	// set rx buffer iterator vars
	rxBufferIndex = 0;
	rxBufferLength = readed;

	return readed;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
}

void TwoWire::beginTransmission(uint8_t address) {
	status = MASTER_SEND;

	// save address of target and empty buffer
	txAddress = address;
	txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
	beginTransmission((uint8_t) address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop) {
	uint8_t ret = 4;
	if (txBufferLength > 0)
    {
        twi->ADDRESS = txAddress;
		// transmit buffer (blocking)
		ret = twi_master_write(txBuffer, txBufferLength, sendStop);
	}
	// reset tx buffer iterator vars
	//txBufferIndex = 0;
	txBufferLength = 0;
	// indicate that we are done transmitting
	status = MASTER_IDLE;
	return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

size_t TwoWire::write(uint8_t data) {
	if (status == MASTER_SEND) {
		if (txBufferLength >= BUFFER_LENGTH)
			return 0;
		txBuffer[txBufferLength++] = data;
		return 1;
	} else {
		if (srvBufferLength >= BUFFER_LENGTH)
			return 0;
		srvBuffer[srvBufferLength++] = data;
		return 1;
	}
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
	if (status == MASTER_SEND) {
		for (size_t i = 0; i < quantity; ++i) {
			if (txBufferLength >= BUFFER_LENGTH)
				return i;
			txBuffer[txBufferLength++] = data[i];
		}
	} else {
		for (size_t i = 0; i < quantity; ++i) {
			if (srvBufferLength >= BUFFER_LENGTH)
				return i;
			srvBuffer[srvBufferLength++] = data[i];
		}
	}
	return quantity;
}

int TwoWire::available(void) {
	return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void) {
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex++];
	return -1;
}

int TwoWire::peek(void) {
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex];
	return -1;
}

void TwoWire::flush(void) {
	// Do nothing, use endTransmission(..) to force
	// data transfer.
}

// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
  // don't bother if user hasn't registered a callback
  if(!onReceiveCallback){
    return;
  }
  // don't bother if rx buffer is in use by a master requestFrom() op
  // i know this drops data, but it allows for slight stupidity
  // meaning, they may not have read all the master requestFrom() data yet
  if(rxBufferIndex < rxBufferLength){
    return;
  }
  // copy twi rx buffer into local read buffer
  // this enables new reads to happen in parallel
  for(uint8_t i = 0; i < numBytes; ++i){
    rxBuffer[i] = inBytes[i];
  }
  // set rx iterator vars
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  // alert user program
  onReceiveCallback(numBytes);
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
  // don't bother if user hasn't registered a callback
  if(!onRequestCallback){
    return;
  }
  // reset tx buffer iterator vars
  // !!! this will kill any pending pre-master sendTo() activity
  //txBufferIndex = 0;
  txBufferLength = 0;
  // alert user program
  onRequestCallback();
}

void TwoWire::onReceive(void(*function)(int)) {
	onReceiveCallback = function;
}

void TwoWire::onRequest(void(*function)(void)) {
	onRequestCallback = function;
}
/*
void TwoWire::onService(void) {
	// Retrieve interrupt status
	uint32_t sr = TWI_GetStatus(twi);

	if (status == SLAVE_IDLE && TWI_STATUS_SVACC(sr)) {
		TWI_DisableIt(twi, TWI_IDR_SVACC);
		TWI_EnableIt(twi, TWI_IER_RXRDY | TWI_IER_GACC | TWI_IER_NACK
				| TWI_IER_EOSACC | TWI_IER_SCL_WS | TWI_IER_TXCOMP);

		srvBufferLength = 0;
		srvBufferIndex = 0;

		// Detect if we should go into RECV or SEND status
		// SVREAD==1 means *master* reading -> SLAVE_SEND
		if (!TWI_STATUS_SVREAD(sr)) {
			status = SLAVE_RECV;
		} else {
			status = SLAVE_SEND;

			// Alert calling program to generate a response ASAP
			if (onRequestCallback)
				onRequestCallback();
			else
				// create a default 1-byte response
				write((uint8_t) 0);
		}
	}

	if (status != SLAVE_IDLE) {
		if (TWI_STATUS_TXCOMP(sr) && TWI_STATUS_EOSACC(sr)) {
			if (status == SLAVE_RECV && onReceiveCallback) {
				// Copy data into rxBuffer
				// (allows to receive another packet while the
				// user program reads actual data)
				for (uint8_t i = 0; i < srvBufferLength; ++i)
					rxBuffer[i] = srvBuffer[i];
				rxBufferIndex = 0;
				rxBufferLength = srvBufferLength;

				// Alert calling program
				onReceiveCallback( rxBufferLength);
			}

			// Transfer completed
			TWI_EnableIt(twi, TWI_SR_SVACC);
			TWI_DisableIt(twi, TWI_IDR_RXRDY | TWI_IDR_GACC | TWI_IDR_NACK
					| TWI_IDR_EOSACC | TWI_IDR_SCL_WS | TWI_IER_TXCOMP);
			status = SLAVE_IDLE;
		}
	}

	if (status == SLAVE_RECV) {
		if (TWI_STATUS_RXRDY(sr)) {
			if (srvBufferLength < BUFFER_LENGTH)
				srvBuffer[srvBufferLength++] = TWI_ReadByte(twi);
		}
	}

	if (status == SLAVE_SEND) {
		if (TWI_STATUS_TXRDY(sr) && !TWI_STATUS_NACK(sr)) {
			uint8_t c = 'x';
			if (srvBufferIndex < srvBufferLength)
				c = srvBuffer[srvBufferIndex++];
			TWI_WriteByte(twi, c);
		}
	}
}*/

TwoWire Wire = TwoWire(WIRE_INTERFACE);

