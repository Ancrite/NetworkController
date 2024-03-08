#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "socket.h"
#include "MQTTPacket.h"
#include "StackTrace.h"

uint8_t server_ip[4];            // 配置远程服务器IP地址
uint16_t server_port = 1883;     // 配置远程服务器端口
uint16_t local_port = 6000;      // 初始化一个本地端口
uint8_t  msgbuf[1024];

uint8_t CONNECT_FLAG = 0;
uint8_t SUB_FLAG = 0;
char ser_cmd[240];

unsigned char dup;
int qos;
unsigned char retained;
unsigned short mssageid;
int payloadlen_in;
unsigned char* payload_in;
MQTTString receivedTopic;
char topic[100] = { "/TKKMt4nMF8U/MQTT1/mqtt" };//设置发布订阅主题
char new_topic[100];
int count = 0;
char rebuf[1024];

/*****************拼接连接报文*****************/
void make_con_msg(char* clientID, int keepalive, uint8_t cleansession, char* username, char* password, unsigned char* buf, int buflen)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = clientID;
    data.keepAliveInterval = keepalive;
    data.cleansession = cleansession;
    data.username.cstring = username;
    data.password.cstring = password;
    MQTTSerialize_connect(buf, buflen, &data);//构造链接报文
    return;
}

/*****************拼接订阅报文*****************/
void make_sub_msg(char* Topic, unsigned char* msgbuf, int buflen)
{
    int msgid = 1;
    int req_qos = 0;
    unsigned char topic[100];
    MQTTString topicString = MQTTString_initializer;
    memcpy(topic, Topic, strlen(Topic));
    topicString.cstring = (char*)topic;
    //topicString.lenstring.len=4;
    MQTTSerialize_subscribe(msgbuf, buflen, 0, msgid, 1, &topicString, &req_qos);
    return;
}

/*****************拼接发布报文*****************/
void make_pub_msg(char* Topic, unsigned char* msgbuf, int buflen, char* msg)
{
    unsigned char topic[100];
    int msglen = strlen(msg);
    MQTTString topicString = MQTTString_initializer;
    memset(topic, 0, sizeof(topic));
    memcpy(topic, Topic, strlen(Topic));
    topicString.cstring = (char*)topic;
    MQTTSerialize_publish(msgbuf, buflen, 0, 2, 0, 0, topicString, (unsigned char*)msg, msglen);
    return;
}

/*****************拼接PING报文*****************/
int  make_ping_msg(unsigned char* buf, int buflen)
{
    return MQTTSerialize_pingreq(buf, buflen);
}

/*****************解析收到的ACK报文*****************/
int mqtt_decode_msg(unsigned char* buf)
{
    int rc = -1;
    MQTTHeader header = { 0 };
    header.byte = buf[0];
    rc = header.bits.type;
    return rc;
}

void MQTT_Connect(void)
{
    int len;
    switch (getSn_SR(MQTTSOC))                      // 获取socket0的状态
    {
    case SOCK_INIT:                                 // Socket处于初始化完成(打开)状态
        connect(MQTTSOC, server_ip, server_port);   // 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
        break;
    case SOCK_ESTABLISHED:                          // Socket处于连接建立状态
        if (getSn_IR(MQTTSOC) & Sn_IR_CON) {
            setSn_IR(MQTTSOC, Sn_IR_CON);           // Sn_IR的CON位置1，通知W5500连接已建立
        }
        memset(msgbuf, 0, sizeof(msgbuf));
        if ((len = getSn_RX_RSR(MQTTSOC)) == 0) {
            /* 连接 */
            if (0 == CONNECT_FLAG) {
                printf("connecting...\r\n");

                /*MQTT拼接连接报文*/
                make_con_msg("NetCtrlClient", 180, 1, NULL, NULL, msgbuf, sizeof(msgbuf));
                send(MQTTSOC, msgbuf, sizeof(msgbuf));

                HAL_Delay(2000);
                if ((len = getSn_RX_RSR(0)) == 0) {
                    break;
                }

                recv(MQTTSOC, msgbuf, len);
                if (mqtt_decode_msg(msgbuf) != CONNACK) {//判断是不是CONNACK
                    printf("connect failed.. waiting retry.\r\n");
                    break;
                }
                CONNECT_FLAG = 1;
                printf("==== connect successed!! ====");
            }
            /* 订阅 */
            else if (0 == SUB_FLAG) {
                printf("subscribing...\r\n");
                memset(msgbuf, 0, sizeof(msgbuf));

                /*MQTT拼接订阅报文*/
                make_sub_msg(topic, msgbuf, sizeof(msgbuf));
                send(MQTTSOC, msgbuf, sizeof(msgbuf));// 接收到数据后再回给服务器，完成数据回环

                HAL_Delay(2000);
                if ((len = getSn_RX_RSR(0)) == 0) {
                    break;
                }
                recv(MQTTSOC, msgbuf, len);
                if (mqtt_decode_msg(msgbuf) != SUBACK) { //判断是不是SUBACK
                    printf("subscribe failed.. waiting retry.\r\n");
                    break;
                }
                SUB_FLAG = 1;
                printf("==== subscribe successed!! ====");
            }
            /* 保活 */
            else {
                if (count > 10000) {
                    count = 0;
                    make_ping_msg(msgbuf, sizeof(msgbuf));
                    send(MQTTSOC, msgbuf, sizeof(msgbuf));

                    while ((len = getSn_RX_RSR(0)) == 0) {
                        printf("wait pingresponse");
                    }
                    recv(0, msgbuf, len);
                    if (mqtt_decode_msg(msgbuf) != PINGRESP) {
                        MQTTSerialize_disconnect(msgbuf, sizeof(msgbuf));
                        send(MQTTSOC, msgbuf, sizeof(msgbuf));
                        CONNECT_FLAG = 0;
                        SUB_FLAG = 0;
                        break;
                    }
                }
            }
        }
        break;
    case SOCK_CLOSE_WAIT:                           // Socket处于等待关闭状态
        close(MQTTSOC);                                   // 关闭Socket0
        break;
    case SOCK_CLOSED:                               // Socket处于关闭状态
        socket(MQTTSOC, Sn_MR_TCP, local_port, Sn_MR_ND); // 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
        break;
    }
}

void MQTT_Subscribe()
{

}