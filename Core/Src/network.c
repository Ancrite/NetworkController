#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "network.h"
#include "spi.h"
#include "flash.h"
#include "clib.h"
#include "dhcp.h"
#include "dns.h"
//#include <MQTTPacket.h>

uint8_t net_flag = 0;

uint8_t gDATABUF_set[DATA_BUF_SIZE];
uint8_t gDATABUF_dhcp[DATA_BUF_SIZE];
uint8_t gDATABUF_dns[DATA_BUF_SIZE];

/* 默认网络参数 */
wiz_NetInfo gWIZNETINFO = { .mac = {0,0,0,0,0,0},
                            .ip = {192,168,6,10},
                            .sn = {255,255,255,0},
                            .gw = {192,168,6,1},
                            .dns = {8,8,8,8},
                            .dhcp = NETINFO_STATIC };

/* DNS */
uint8_t DNS_ip[4] = { 8, 8, 8, 8 };

void Wizchip_Callback_Init(void)
{
    reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);
    reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);
    reg_wizchip_spi_cbfunc(SPI_Read_Byte, SPI_Write_Byte);
#ifdef DHCP_CONF
    reg_dhcp_cbfunc(my_ip_assign, my_ip_update, my_ip_conflict);
#endif
}

void Network_UserConfig_Init(userparameter_t local_config, uint8_t config_valid_flag)
{
    /* 获取存储的配置并校验有效性 */
    BSP_FLASH_GetUserParameters(&local_config);
    config_valid_flag = (local_config.crc == clib_crc_8((uint8_t*)&local_config, sizeof(userparameter_t) - 1)) ? 1 : 0;

    /* 载入配置 */
    if (config_valid_flag) {
        clib_memcpy(gWIZNETINFO.ip, local_config.local_ipaddr, 4, 0);
        clib_memcpy(gWIZNETINFO.sn, local_config.local_netmask, 4, 0);
        clib_memcpy(gWIZNETINFO.gw, local_config.local_gwaddr, 4, 0);
        // BSP_USART_S2EConfig(&user_config);  //串口配置
    }
}

void Network_Init(void)
{
    uint8_t  memsize[2][8] = { {2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2} };
    uint32_t macHigh, macLow;

    //MAC address init
    srand((unsigned int)HAL_GetUIDw0());
    macHigh = (uint32_t)rand();
    macLow = (uint32_t)rand();
    gWIZNETINFO.mac[0] = (macHigh >> 8) & 0xfe; //clear multicast bit
    gWIZNETINFO.mac[1] = macHigh | 0x02;        //set local assignment bit (IEEE802)
    gWIZNETINFO.mac[2] = macLow >> 24;
    gWIZNETINFO.mac[3] = macLow >> 16;
    gWIZNETINFO.mac[4] = macLow >> 8;
    gWIZNETINFO.mac[5] = macLow;
    setSHAR(gWIZNETINFO.mac);

    //socket buffer init
    if (ctlwizchip(CW_INIT_WIZCHIP, (void*)memsize) == -1) {
        printf("WIZCHIP Initialized fail.\r\n");
        while (1);
    }

    //network information init
#ifdef DHCP_CONF
    gWIZNETINFO.dhcp = NETINFO_DHCP;
    DHCP_init(SOC_DHCP, gDATABUF_dhcp);
#else
    if ((ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO))) {
        printf("IPv4 Initialized fail.\r\n");
    }
    setRTR(2000);
    setRCR(3);
    net_flag = 1;

    // Display Network Information
    ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
    printf("\r\n=== STATIC NET CONF ===\r\n");
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2],
        gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
    printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
    printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
    printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
    printf("=======================\r\n");
#endif

    //dns client init
    DNS_init(SOC_DNS, gDATABUF_dns);
}

void Network_UserConfig_SettingDaemon(uint8_t sn, userparameter_t* local_config, uint8_t config_valid_flag)
{
    uint8_t  setting_ip[4];
    uint16_t setting_port;
    uint8_t  udp_ip[4] = { 255,255,255,255 };
    uint16_t udp_port = 50900;
    int16_t  recvlen = 0;

    switch (getSn_SR(sn)) {
        case SOCK_UDP:                               // Socket处于初始化完成(打开)状态
            /* 接收到数据包 */
            if ((recvlen = getSn_RX_RSR(0)) > 0) {
                const char* tag = "hi module";
                memset(gDATABUF_set, 0, recvlen + 1);
                recvfrom(0, gDATABUF_set, recvlen, setting_ip, &setting_port);

                /* 响应 “搜索/刷新”('hi module') */
                if (clib_memcmp((uint8_t*)gDATABUF_set, (uint8_t*)tag, strlen(tag))) {

                    /* 回显数据包中写入当前MAC地址和CRC校验值 */
                    if (!config_valid_flag) {
                        clib_memcpy((uint8_t*)local_config, (uint8_t*)gWIZNETINFO.mac, 6, 0);
                        local_config->crc = clib_crc_8((uint8_t*)local_config, sizeof(userparameter_t) - 1);
                    }

                    /* 当前参数回环显示 */
                    sendto(sn, (uint8_t*)local_config, sizeof(userparameter_t), udp_ip, udp_port);
                }

                /* 响应 “上传参数” */
                if (clib_memcmp(gWIZNETINFO.mac, (uint8_t*)gDATABUF_set, 6)) {
                    userparameter_t flash_config;

                    /* 将包含新的参数的数据包拷贝到临时变量中并校验 */
                    clib_memcpy((uint8_t*)&flash_config, (uint8_t*)gDATABUF_set, sizeof(userparameter_t), 0);
                    if (flash_config.crc != clib_crc_8((uint8_t*)&flash_config, sizeof(userparameter_t) - 1)) {
                        return;
                    }

                    /* 将新的配置参数写入 FLASH */
                    if (BSP_FLASH_SetUserParameters(&flash_config)) {
                        printf("[ %s, %d ] fail to configurate the new parameters.", __FILE__, __LINE__);
                    }

                    printf("success to config the new parameters.\n");
                    printf("Prepare to reboot the MCU, Please wait a minute..... ^_^\n");

                    __ASM volatile ("cpsid i");
                    HAL_NVIC_SystemReset();
                }
            }
            break;
        case SOCK_CLOSED:                            // Socket处于关闭状态
            socket(sn, Sn_MR_UDP, 8000, 0x00);
            break;
        default:
            break;
    }
}

void Network_UDP_ServerDaemon(uint8_t sn)
{
}

void Network_TCP_ClientDaemon(uint8_t sn)
{
    uint8_t  TCP_DIP[4] = { 192,168,50,27 };
    uint16_t TCP_DPort = 4211;

    switch (getSn_SR(sn)) {
    case SOCK_INIT:    //TCP初始化
        connect(sn, TCP_DIP, TCP_DPort);
        break;
    case SOCK_ESTABLISHED:
        break;
    case SOCK_CLOSE_WAIT:   //等待关闭
        disconnect(sn);
        break;
    case SOCK_CLOSED:   //打开本地端口
        socket(sn, Sn_MR_TCP, 1126, 0x00);
        break;
    }
}

void DHCP_ClientDaemon(void)
{
    static uint8_t cnt_retry = 0;
    switch (DHCP_run()) {
    case DHCP_IP_LEASED:
        // NETWORK APPs.
        if (0 == net_flag) {
            net_flag = 1;
        }
        break;
    case DHCP_FAILED:
        // if omitted, retry to process DHCP
        cnt_retry++;
        if (cnt_retry > 3) {
            printf(">> DHCP %d Failed\r\n", cnt_retry);
            cnt_retry = 0;
        }
        break;
    default:
        break;
    }
}

void my_ip_assign(void)
{
    default_ip_assign();

    ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
    printf("\r\n=== DHCP NET CONF ===\r\n");
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2],
        gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
    printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
    printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
    printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
    printf("======================\r\n");

    net_flag = 1;
}

void my_ip_update(void)
{
    default_ip_update();
}

void my_ip_conflict(void)
{
    default_ip_conflict();
}

//phy link status check
uint8_t Network_PHYLink_Check(void)
{
    uint8_t  tmp;
    if (ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1) {
        printf("Unknown PHY Link stauts.\r\n");
        return -1;
    }
    return tmp;
}

uint8_t Get_Domin_IP(uint8_t* domain_name, uint8_t* domain_ip)
{
    int ret = 0;
    if ((ret = DNS_run(DNS_ip, domain_name, domain_ip)) > 0) {
        printf("> 1st DNS Reponsed\r\n");
    }
    else if (ret == -1) {
        printf("> MAX_DOMAIN_NAME is too small. Should be redefined it.\r\n");
    }
    else {
        printf("> DNS Failed\r\n");
    }

    if (ret > 0) {
        printf("> Translated %s to %d.%d.%d.%d\r\n", domain_name, domain_ip[0], domain_ip[1], domain_ip[2], domain_ip[3]);
    }
    return ret;
}