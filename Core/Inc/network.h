#ifndef __NETWORK_H
#define __NETWORK_H

#include "main.h"
#include "socket.h"

//#define INFO_OUTPUT
#define DHCP_CONF
#define DATA_BUF_SIZE 2048

#define SOC_SETTING 0
#define SOC_TCPS    1
#define SOC_DHCP    2
#define SOC_DNS     3
#define SOC_MQTT    4

typedef struct
{
    uint8_t macaddr[6];
    uint8_t target_ipaddr[4];
    uint8_t target_port[2];
    uint8_t local_mode;
    uint8_t local_ipaddr[4];
    uint8_t local_gwaddr[4];
    uint8_t local_netmask[4];
    uint8_t local_port[2];
    uint8_t usart_para[4];
    uint8_t crc;
} __packed userparameter_t;

extern uint8_t net_flag;

void Wizchip_Callback_Init(void);
void Network_UserConfig_Init(userparameter_t local_config, uint8_t config_valid_flag);

/**
 * @brief Network initialized function
 *
 */
void Network_Init(void);

void Network_UserConfig_SettingDaemon(uint8_t sn, userparameter_t* local_config, uint8_t config_valid_flag);
void Network_UDP_ServerDaemon(uint8_t sn);
void Network_TCP_ClientDaemon(uint8_t sn);
void DHCP_ClientDaemon(void);

void my_ip_assign(void);
void my_ip_update(void);
void my_ip_conflict(void);

extern void default_ip_assign(void);
extern void default_ip_update(void);
extern void default_ip_conflict(void);

uint8_t Network_PHYLink_Check(void);
uint8_t Get_Domin_IP(uint8_t* domain_name, uint8_t* domain_ip);
#endif
