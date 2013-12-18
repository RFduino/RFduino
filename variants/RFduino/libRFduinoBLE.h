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

// default "RFduino"
extern const char *RFduinoBLE_device_name;

// default "sketch"
extern const char *RFduinoBLE_advertisement_data;

// in ms (converted to 0.625ms units) - range 20ms to 10.24s - default 80ms
extern int RFduinoBLE_advertisement_interval;

// -20 dBm to +4 dBm - default +4 dBm
extern int8_t RFduinoBLE_tx_power_level;

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
int RFduinoBLE_begin(void);

void RFduinoBLE_end(void);

bool RFduinoBLE_send(const char *data, uint8_t len);

// void RFduinoBLE_wait_for_event(void);

#ifdef __cplusplus
}
#endif

#endif
