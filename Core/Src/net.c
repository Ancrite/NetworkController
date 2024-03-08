#include "net.h"
#include "clib.h"
#include "flash.h"
#include "socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTPacket.h>

uint8_t  TCP_DIP[4] = { 192,168,50,27 };
uint16_t TCP_DPort = 4211;
#ifdef MULTICAST
uint8_t DIP[4] = { 255,255,255,255 };
uint8_t DHAR[6] = { 0x01,0x00,0x5e,0xff,0xff,0xff };
uint16_t DPORT = 8000;
#endif

void NET_Init(void)
{
    uint8_t  memsize[2][8] = { {2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2} };
    uint8_t  tmp;
    uint8_t  tmpstr[6];
    uint32_t macHigh, macLow;

    //socket buffer init
    if (ctlwizchip(CW_INIT_WIZCHIP, (void*)memsize) == -1) {
        printf("WIZCHIP Initialized fail.\r\n");
        while (1);
    }

    //phy link status check
    do {
        if (ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1) {
            printf("Unknown PHY Link stauts.\r\n");
        }
    } while (tmp == PHY_LINK_OFF);

    //IPv4 Initialized.
    srand((unsigned int)HAL_GetUIDw0);
    macHigh = (uint32_t)rand();
    macLow = (uint32_t)rand();
    gWIZNETINFO.mac[0] = (macHigh >> 8) & 0xfe; //clear multicast bit
    gWIZNETINFO.mac[1] = macHigh | 0x02;        //set local assignment bit (IEEE802)
    gWIZNETINFO.mac[2] = macLow >> 24;
    gWIZNETINFO.mac[3] = macLow >> 16;
    gWIZNETINFO.mac[4] = macLow >> 8;
    gWIZNETINFO.mac[5] = macLow;
    if ((ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO))) {
        printf("IPv4 Initialized fail.\r\n");
    }
    setRTR(2000);
    setRCR(3);

#ifdef INFO_OUTPUT
    // Display Network Information
    ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
    ctlwizchip(CW_GET_ID, (void*)tmpstr);
    printf("\r\n=== %s NET CONF ===\r\n", (char*)tmpstr);
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2],
                                                     gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
    printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
    printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
    printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
    printf("======================\r\n");
#endif
}

void NET_UDP_ServerDaemon(userparameter_t* local_config)
{
    uint8_t udp_ip[4] = { 255,255,255,255 };
    uint16_t udp_port;
    int16_t recvlen = 0;
    /* 接收到数据包 */
    if ((recvlen = getSn_RX_RSR(0)) > 0) {
        memset(gDATABUF, 0, recvlen + 1);
        recvfrom(0, gDATABUF, recvlen, udp_ip, &udp_port);

        /* 另存为pbuf格式 */
        pbuf* recvbuf = (pbuf *)malloc(sizeof(pbuf));
        clib_memcpy((uint8_t*)recvbuf, gDATABUF, sizeof(pbuf), 0);

        /* 响应上传参数 */
        if (recvbuf->tot_len == sizeof(userparameter_t) && clib_memcmp(gWIZNETINFO.mac, (uint8_t*)recvbuf->payload, 6)) {
            userparameter_t tmp_config;

            /* 将包含新的参数的数据包拷贝到临时变量中并校验 */
            clib_memcpy((uint8_t*)&tmp_config, (uint8_t*)recvbuf->payload, sizeof(userparameter_t), 0);
            if (tmp_config.crc != clib_crc_8((uint8_t*)&tmp_config, sizeof(userparameter_t) - 1)) {
                return;
            }
            /* 将新的配置参数写入 FLASH */
            if (BSP_FLASH_SetUserParameters(&tmp_config)) {
                printf("[ %s, %d ] fail to configurate the new parameters.", __FILE__, __LINE__);
            }
            free(recvbuf);

            printf("success to config the new parameters.\n");
            printf("Prepare to reboot the MCU, Please wait a minute..... ^_^\n");

            __ASM volatile ("cpsid i");
            HAL_NVIC_SystemReset();
        }

        const char* tag = "hi module";
        /* 响应 'hi module' */
        if (recvbuf->tot_len == strlen(tag) && clib_memcmp((uint8_t*)recvbuf->payload, (uint8_t*)tag, strlen(tag))) {
            // pbuf* reply = (pbuf *)malloc(sizeof(pbuf));
            // if (reply == NULL) {
            //     printf("[ %s, %d ]pbuf allocation failure.\n", __FILE__, __LINE__);
            // }

            /* 更改配置参数前更新校验值 */
            if (!config_valid_flag) {
                clib_memcpy((uint8_t*)local_config, (uint8_t*)gWIZNETINFO.mac, 6, 0);
                local_config->crc = clib_crc_8((uint8_t*)local_config, sizeof(userparameter_t) - 1);
            }
            /* 当前参数回环显示 */
            // clib_memcpy((uint8_t*)reply->payload, (uint8_t*)local_config, sizeof(userparameter_t), 0);
            sendto(0, (uint8_t*)local_config, sizeof(userparameter_t), udp_ip, udp_port);

            free(recvbuf);
            // free(reply);
        }
    }
}

void NET_Setting_ServerDaemon(uint8_t sn)
{
    switch (getSn_SR(sn)) {
        case SOCK_UDP:                               // Socket处于初始化完成(打开)状态
            HAL_Delay(100);
            NET_UDP_ServerDaemon(&user_config);
        case SOCK_CLOSED:                            // Socket处于关闭状态
            socket(sn, Sn_MR_UDP, 8000, 0x00);
            break;
    }
}

void NET_TCP_ClientDaemon(uint8_t sn)
{
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