#ifndef _DEMO__
#define _DEMO__

#ifdef __cplusplus
extern "C" {
#endif

void demo_ble_scan_init(void);

void demo_ble_advertise_init(void);

void demo_ble_advertist_stop(void);

void demo_ble_advertist_start(void);


void demo_ble_ext_advertise_init(void);


void demo_ble_gatt_server_init(void);

void demo_ble_gatt_client_init(void);

#ifdef __cplusplus
}
#endif

#endif 
