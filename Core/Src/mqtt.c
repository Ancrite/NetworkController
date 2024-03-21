#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tim.h"
#include "mqtt.h"
#include "at.h"
#include "socket.h"
#include "MQTTPacket.h"
#include "StackTrace.h"

uint8_t server_ip[4];            // 配置远程服务器IP地址
uint16_t server_port = 1883;     // 配置远程服务器端口
uint16_t local_port = 6000;      // 初始化一个本地端口
uint8_t  msgbuf[1024];

uint8_t CONNECT_FLAG = 0;
uint8_t SUB_FLAG = 0;
int count = 0;

static void make_con_msg(char* clientID, int keepalive, uint8_t cleansession, char* username, char* password, unsigned char* buf, int buflen)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = clientID;
    data.keepAliveInterval = keepalive;
    data.cleansession = cleansession;
    data.username.cstring = username;
    data.password.cstring = password;
    MQTTSerialize_connect(buf, buflen, &data);
    return;
}

static void make_sub_msg(char* Topic, unsigned char* msgbuf, int buflen)
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

static void make_pub_msg(char* Topic, unsigned char* msgbuf, int buflen, char* msg)
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

static int make_ping_msg(unsigned char* buf, int buflen)
{
    return MQTTSerialize_pingreq(buf, buflen);
}

static int mqtt_decode_msg(unsigned char* buf)
{
    int rc = -1;
    MQTTHeader header = { 0 };
    header.byte = buf[0];
    rc = header.bits.type;
    return rc;
}
static int mqtt_wait_ack(mqtt_client_t* mqtt_client, int ack_type, TIM_HandleTypeDef* timer, uint16_t timeout)
{
    int rc = MQTT_FAILED_ERROR;
    HAL_TIM_Base_Start_IT(timer);
    while (count <= timeout) {
        if (getSn_RX_RSR(mqtt_client->mqtt_sn)) {
            recv(mqtt_client->mqtt_sn, mqtt_client->mqtt_read_buf, 1024);
            if (mqtt_decode_msg(mqtt_client->mqtt_read_buf) == ack_type) {
                rc = MQTT_SUCCESS;
                break;
            }
            else {
                rc = MQTT_PUBLISH_ACK_TYPE_ERROR;
            }
        }
        else {
            rc = MQTT_PUBLISH_ACK_PACKET_ERROR;
        }
    }
    HAL_TIM_Base_Stop_IT(timer);
    count = 0;
    return rc;
}

/******************* API *******************/
void MQTT_Init(mqtt_client_t* mqtt_client)
{
    // 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
    socket(mqtt_client->mqtt_sn, Sn_MR_TCP, local_port, Sn_MR_ND);

    // 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
    connect(mqtt_client->mqtt_sn, mqtt_client->mqtt_host, mqtt_client->mqtt_port);
}

void MQTT_Reconnect(mqtt_client_t* mqtt_client)
{
    while (!CONNECT_FLAG)
    {
        CONNECT_FLAG = 0;
        SUB_FLAG = 0;
        memset(mqtt_client->mqtt_write_buf, 0, mqtt_client->mqtt_write_buf_len);

        /*拼接连接报文*/
        make_con_msg(   mqtt_client->mqtt_clientid,
                        mqtt_client->mqtt_keepalive_interval,
                        mqtt_client->mqtt_cleansession,
                        mqtt_client->mqtt_username,
                        mqtt_client->mqtt_password,
                        mqtt_client->mqtt_write_buf,
                        mqtt_client->mqtt_write_buf_len);
        send(mqtt_client->mqtt_sn, mqtt_client->mqtt_write_buf, mqtt_client->mqtt_write_buf_len);

        /* 回复报文超时判断 */
        if (mqtt_wait_ack(mqtt_client, CONNACK, &htim2, 200) == 0) {
            CONNECT_FLAG = 1;
        }
    }
}

void MQTT_Disconnect(mqtt_client_t* mqtt_client)
{}

void MQTT_Keepalive(mqtt_client_t* mqtt_client)
{}

void MQTT_Subscribe(mqtt_client_t* mqtt_client, const char* topic)
{
    memset(mqtt_client->mqtt_write_buf, 0, mqtt_client->mqtt_write_buf_len);

    /*MQTT拼接订阅报文*/
    make_sub_msg((char*)topic, mqtt_client->mqtt_write_buf, sizeof(mqtt_client->mqtt_write_buf));
    send(mqtt_client->mqtt_sn, mqtt_client->mqtt_write_buf, sizeof(mqtt_client->mqtt_write_buf));

    if (!mqtt_wait_ack(mqtt_client, SUBACK, &htim2, 200))
        SUB_FLAG = 1;
}

void MQTT_Unsubscribe(mqtt_client_t* mqtt_client, const char* topic)
{}

void MQTT_Publish(mqtt_client_t* mqtt_client, const char* topic, uint8_t* msgbuf)
{
}

void MQTT_Receivehandle(mqtt_client_t* mqtt_client)
{
    int len;
    unsigned char dup;
    int qos;
    MQTTString receivedTopic;
    unsigned char retained;
    unsigned char* payload;
    int payloadlen;
    if ((len = getSn_RX_RSR(mqtt_client->mqtt_sn)) > 0) {
        recv(mqtt_client->mqtt_sn, mqtt_client->mqtt_read_buf, len);
        if (PUBLISH == mqtt_decode_msg(mqtt_client->mqtt_read_buf)) {
            MQTTDeserialize_publish(&dup,
                                    &qos,
                                    &retained,
                                    &mqtt_client->mqtt_packet_id,
                                    &receivedTopic,
                                    &payload,
                                    &payloadlen,
                                    mqtt_client->mqtt_read_buf,
                                    len);
            strncpy((char*)msgbuf, (const char*)payload, payloadlen);
        }
        else if (PINGRESP == mqtt_decode_msg(mqtt_client->mqtt_read_buf)) {
            if (len > 2) {
                if (PUBLISH == mqtt_decode_msg(mqtt_client->mqtt_read_buf + 2)) {
                    printf("publish\r\n");
                    MQTTDeserialize_publish(&dup,
                                            &qos,
                                            &retained,
                                            &mqtt_client->mqtt_packet_id,
                                            &receivedTopic,
                                            &payload,
                                            &payloadlen,
                                            mqtt_client->mqtt_read_buf + 2,
                                            len - 2);
                    strncpy((char*)msgbuf, (const char*)payload, payloadlen);
                }
            }
        }
        else {
            printf("wait publish\r\n");
        }
    }
}