#ifndef __RELAY_H
#define __RELAY_H
#include "main.h"

#define RELAY1 0x01
#define RELAY2 0x02
#define RELAY3 0x04
#define RELAY4 0x08
#define RELAY5 0x10
#define RELAY6 0x20
#define RELAY7 0x40
#define RELAY8 0x80

void Relay_Pullin(uint8_t port);
void Relay_Release(uint8_t port);
#endif