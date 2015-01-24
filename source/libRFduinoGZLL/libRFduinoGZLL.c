/*
  libRFduinoGZLL.c

  Copyright (c) 2014 RF Digital Corporation. All Rights Reserved.

  The information contained herein is property of RF Digital
  Corporation and is free for use on any product or in any
  application containing an RF Digital Module.

  NO WARRANTY of ANY KIND is provided. This heading must NOT be
  removed from the file.
*/

/*
The Gazell protocol is a wireless communication protocol that is used
to setup a robust wireless link between a single host and up to eight
devices in a star network topology.

The Host in a Gazell network is always listening, and it is the Device
that always initiates a communication.

Each packet that a Device sends is required to be acknowledged by the
Host.  Gazell auotmatically handle packet retransmission if necessary.

It is possible for the Host to send data to the Device by piggybacking
data to an acknowledgement (ACK) packet.  Therefore a Host has to wait
for a packet from a Device before it can send any data to it.

Gazell utilizes channel hopping functionality that gives a high date
rate and reliable wireless link.

Gazell requires no connection packets to setup a link, and devices can
enter and exit from the network at any time.
*/

#include <stdio.h>

#include "nrf_gzll.h"

#include "libRFduinoGZLL.h"

extern int RFduinoGZLL_enabled;

// -20 dBm to +4 dBm = default +4 dBm
int RFduinoGZLL_tx_power_level = 4;
uint32_t RFduinoGZLL_host_base_address = 0U;
uint32_t RFduinoGZLL_device_base_address = 0U;

static device_t _device;


// 0 = success
// 1 = init failed
// 2 = set tx_power failed
// 3 = enabled failed
// 4 = set channel selection policy failed
int RFduinoGZLL_begin(device_t device)
{
  _device = device;

  if (! nrf_gzll_init(device == HOST ? NRF_GZLL_MODE_HOST : NRF_GZLL_MODE_DEVICE))
    return 1;

  nrf_gzll_tx_power_t tx_power;
  if (RFduinoGZLL_tx_power_level <= -20)
    tx_power = NRF_GZLL_TX_POWER_N20_DBM;
  else if (RFduinoGZLL_tx_power_level <= -16)
    tx_power = NRF_GZLL_TX_POWER_N16_DBM;
  else if (RFduinoGZLL_tx_power_level <= -12)
    tx_power = NRF_GZLL_TX_POWER_N12_DBM;
  else if (RFduinoGZLL_tx_power_level <= -16)
    tx_power = NRF_GZLL_TX_POWER_N16_DBM;
  else if (RFduinoGZLL_tx_power_level <= -8)
    tx_power = NRF_GZLL_TX_POWER_N8_DBM;
  else if (RFduinoGZLL_tx_power_level <= -4)
    tx_power = NRF_GZLL_TX_POWER_N4_DBM;
  else if (RFduinoGZLL_tx_power_level <= 0)
    tx_power = NRF_GZLL_TX_POWER_0_DBM;
  else
    tx_power = NRF_GZLL_TX_POWER_4_DBM;

  if (! nrf_gzll_set_tx_power(tx_power))
    return 2;

  if (! nrf_gzll_set_device_channel_selection_policy(NRF_GZLL_DEVICE_CHANNEL_SELECTION_POLICY_USE_CURRENT))
    return 4;
 
  if (RFduinoGZLL_host_base_address ) {
   uint8_t msb = RFduinoGZLL_host_base_address >> 24;
   if (msb == 0x55 || msb == 0xAA)
     return 5;  // msb of base address should not be alternating 0s and 1s
	 if ( !nrf_gzll_set_base_address_0(RFduinoGZLL_host_base_address) )
		return 5;
  } 

  if (RFduinoGZLL_device_base_address) {
   uint8_t msb = RFduinoGZLL_device_base_address >> 24;
   if (msb == 0x55 || msb == 0xAA)
     return 6;  // msb of base address should not be alternating 0s and 1s
	 if ( !nrf_gzll_set_base_address_1(RFduinoGZLL_device_base_address) )
		return 6;
  }
	
  if (! nrf_gzll_enable())
    return 3;

  RFduinoGZLL_enabled = 1;

  return 0;
}

bool RFduinoGZLL_send_to_host(const char *data, int len)
{
  if (_device == HOST)
    return false;

  // a zero byte load is valid, but data must be non-null
  if (!data && !len)
    data = &_device;

  return nrf_gzll_add_packet_to_tx_fifo(_device, data, len);
}

bool RFduinoGZLL_send_to_device(device_t device, const char *data, int len)
{
  if (_device != HOST)
    return false;

  return nrf_gzll_add_packet_to_tx_fifo(device, data, len);
}

void RFduinoGZLL_end(void)
{
  nrf_gzll_disable();

  while (nrf_gzll_is_enabled())
    ;

  NVIC_DisableIRQ(RADIO_IRQn);
  NVIC_DisableIRQ(TIMER2_IRQn);

  RFduinoGZLL_enabled = 0;
}

void nrf_gzll_host_rx_data_ready(uint32_t pipe, nrf_gzll_host_rx_info_t rx_info)
{
  char payload[NRF_GZLL_CONST_MAX_PAYLOAD_LENGTH];
  int length = NRF_GZLL_CONST_MAX_PAYLOAD_LENGTH;

  if (! nrf_gzll_fetch_packet_from_rx_fifo(pipe, payload, &length))
    return;

  if (RFduinoGZLL_onReceive)
     RFduinoGZLL_onReceive(pipe, rx_info.rssi, payload, length);
}

void nrf_gzll_device_tx_success(uint32_t pipe, nrf_gzll_device_tx_info_t tx_info)
{
  char payload[NRF_GZLL_CONST_MAX_PAYLOAD_LENGTH];
  int length = 0;

  if (tx_info.payload_received_in_ack)
  {
    length = NRF_GZLL_CONST_MAX_PAYLOAD_LENGTH;
    if (! nrf_gzll_fetch_packet_from_rx_fifo(pipe, payload, &length))
      return;
  }

  if (RFduinoGZLL_onReceive)
     RFduinoGZLL_onReceive(HOST, tx_info.rssi, payload, length);
}

void nrf_gzll_device_tx_failed(uint32_t pipe, nrf_gzll_device_tx_info_t tx_info)
{
}

void nrf_gzll_disabled()
{
}
