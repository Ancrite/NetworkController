#ifndef __NET_H
#define __NET_H

#include "main.h"
#include "mqtt.h"

#define SOC_SETTING 0
#define SOC_TCPS    1

/**
 * @brief Network initialized function
 *
 */
void NET_Init(void);

void NET_Setting_ServerDaemon(uint8_t sn);

void NET_TCP_ClientDaemon(uint8_t sn);
#endif
