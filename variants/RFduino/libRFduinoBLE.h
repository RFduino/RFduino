/*
  Copyright (c) 2013 RF Digital Corporation. All Rights Reserved.

  The information contained herein is property of RF Digital
  Corporation and is free for use on any product or in any
  application containing an RF Digital Module.

  NO WARRANTY of ANY KIND is provided. This heading must NOT be
  removed from the file.
*/

#ifndef _LIBRFDUINOBLE_H_
#define _LIBRFDUINOBLE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// default NULL (NULL/"" = previous fixed RFduino uuid)
extern const char *RFduinoBLE_custom_uuid;

// default "RFduino"
extern const char *RFduinoBLE_device_name;

// default "sketch"
extern const char *RFduinoBLE_advertisement_data;

// in ms (converted to 0.625ms units) - range 20ms to 10.24s - default 80ms
extern int RFduinoBLE_advertisement_interval;

// default false (false = regular advertising, true = ibeacon advertising)
extern bool RFduinoBLE_ibeacon;

// default iBeacon Proximity UUID
extern uint8_t RFduinoBLE_ibeacon_uuid[16];

// default 0
extern unsigned short RFduinoBLE_ibeacon_major;

// default 0
extern unsigned short RFduinoBLE_ibeacon_minor;

// default 2's complement iBeacon Power Measurement at 1 meter (default is 0xC5 = -59dBm)
extern unsigned char RFduinoBLE_ibeacon_measured_power;

// -20 dBm to +4 dBm - default +4 dBm
extern int8_t RFduinoBLE_tx_power_level;

// use with caution: advanced raw access to the advertisement and scan response packets
extern uint8_t *RFduinoBLE_advdata;
extern uint8_t RFduinoBLE_advdata_len;
extern uint8_t *RFduinoBLE_srdata;
extern uint8_t RFduinoBLE_srdata_len;

// default true (false = nonconnectable advertising)
// (nonconnectable forces a minimum 100ms advertisement_interval, >= 100ms intervals are okay)
extern bool RFduinoBLE_connectable;

extern void RFduinoBLE_onAdvertisement(bool start)     __attribute__((weak));
// received signal strength indication (-0dBm to -127dBm)
extern void RFduinoBLE_onRSSI(int rssi)                __attribute__((weak));
extern void RFduinoBLE_onConnect(void)                 __attribute__((weak));
extern void RFduinoBLE_onDisconnect(void)              __attribute__((weak));
extern void RFduinoBLE_onReceive(char *data, int len)  __attribute__((weak));

// 0 = success
// 1 = device_name + advertisement_data invalid (> 18)
// 2 = tx_power_level invalid (< -20 || > +4)
// 3 = advertisement_interval invalid (< 20ms || > 10.24s)

// Now always returns 0 (success) and does a best effort to accomodate the users request.
// This means that it is no longer necessary to check the return value, and being off
// by one byte doesn't stop the device from advertising.
// on advertisement packet overflow, it will truncate the advertisement data first down
// to a single byte, then it will truncate the device name.  if the tx_power_level is
// outside the limits, its modified to the boundary level value.  the same boundary logic
// is also applied to the advertisement_interval.
int RFduinoBLE_begin(void);

void RFduinoBLE_end(void);

bool RFduinoBLE_send(const char *data, uint8_t len);

// request central role use a different connection interval in the given range
// the central role may reject the request (or even pick a value outside the range)
void RFduinoBLE_update_conn_interval(int min_conn_interval_ms, int max_conn_interval_ms);

int RFduinoBLE_get_conn_interval(void);

#ifdef __cplusplus
}
#endif

#endif
