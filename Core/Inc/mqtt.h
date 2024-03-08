#ifndef  __MQTT_H
#define  __MQTT_H

#include "net.h"

#define MQTTSOC SOC_TCPS

extern uint8_t server_ip[4];
extern uint8_t buffer[2048];
extern uint8_t msgbuf[1024];

void MQTT_Connect(void);

#endif