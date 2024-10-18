#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "MQTTPacket.h"
#include "StackTrace.h"
#include "mqtt.h"
#include "tim.h"
#include "at.h"

uint8_t  server_domian[] = "broker.emqx.io";
uint8_t  server_ip[4] = { 192,168,6,129 };
//uint8_t  server_ip[4] = {0,};    // 配置远程服务器IP地址
uint16_t server_port = 1883;     // 配置远程服务器端口
uint16_t local_port = 6000;      // 初始化一个本地端口
uint8_t  msgbuf[2048];

uint8_t  sub_topic[] = "hb_mqtt_test_topic";
uint8_t  recv_topic[100];

uint8_t mqtt_connect_flag = 0;
uint8_t mqtt_subscribe_flag = 0;
int send_buf_len = 0;
int count = 0;

static int make_con_msg(char* clientID, int keepalive, uint8_t cleansession, char* username, char* password, unsigned char* buf, int buflen)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = clientID;
    data.keepAliveInterval = keepalive;
    data.cleansession = cleansession;
    data.username.cstring = username;
    data.password.cstring = password;
    return MQTTSerialize_connect(buf, buflen, &data);
}

static int make_sub_msg(char* Topic, unsigned char* msgbuf, int buflen)
{
    int msgid = 1;
    int req_qos = 0;
    unsigned char topic[100];
    MQTTString topicString = MQTTString_initializer;
    memcpy(topic, Topic, strlen(Topic));
    topicString.cstring = (char*)topic;
    return MQTTSerialize_subscribe(msgbuf, buflen, 0, msgid, 1, &topicString, &req_qos);
}

static int make_pub_msg(char* Topic, unsigned char* msgbuf, int buflen, char* msg)
{
    unsigned char topic[100];
    int msglen = strlen(msg);
    MQTTString topicString = MQTTString_initializer;
    memset(topic, 0, sizeof(topic));
    memcpy(topic, Topic, strlen(Topic));
    topicString.cstring = (char*)topic;
    return MQTTSerialize_publish(msgbuf, buflen, 0, 2, 0, 0, topicString, (unsigned char*)msg, msglen);
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
    uint8_t len = 0;
    int rc = MQTT_FAILED_ERROR;
    HAL_TIM_Base_Start_IT(timer);
    while (count <= timeout) {
        if ((len = getSn_RX_RSR(mqtt_client->mqtt_sn)) > 0) {
            recv(mqtt_client->mqtt_sn, mqtt_client->mqtt_data_buf, len);
            if (mqtt_decode_msg(mqtt_client->mqtt_data_buf) == ack_type) {
                printf("wait_ack success\n");
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
    if (Get_Domin_IP(server_domian, server_ip) <= 0) {
        return;
    }

    mqtt_client->mqtt_sn = SOC_MQTT;
    mqtt_client->mqtt_host = server_ip;
    mqtt_client->mqtt_port = server_port;
    mqtt_client->mqtt_clientid = "1CBF3BBB876F";
    mqtt_client->mqtt_data_buf = msgbuf;
    mqtt_client->mqtt_data_buf_len = 2048;
    mqtt_client->mqtt_keepalive_interval = 0;
    mqtt_client->mqtt_will_flag = 0;
    mqtt_client->mqtt_cleansession = 0;

    // 打开Socket，并配置为TCP无延时模式，打开一个本地端口
    socket(mqtt_client->mqtt_sn, Sn_MR_TCP, local_port, Sn_MR_ND);

    // 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
    connect(mqtt_client->mqtt_sn, mqtt_client->mqtt_host, mqtt_client->mqtt_port);

    mqtt_connect_flag = 1;

    printf("MQTT_Init Done.\r\n");
}

void MQTT_Reconnect(mqtt_client_t* mqtt_client)
{
    while (mqtt_connect_flag) {
        mqtt_connect_flag = 0;
        mqtt_subscribe_flag = 1;
        memset(mqtt_client->mqtt_data_buf, 0, mqtt_client->mqtt_data_buf_len);

        /*拼接连接报文*/
        send_buf_len = make_con_msg(mqtt_client->mqtt_clientid,
            mqtt_client->mqtt_keepalive_interval,
            mqtt_client->mqtt_cleansession,
            mqtt_client->mqtt_username,
            mqtt_client->mqtt_password,
            mqtt_client->mqtt_data_buf,
            mqtt_client->mqtt_data_buf_len);
        send(mqtt_client->mqtt_sn, mqtt_client->mqtt_data_buf, send_buf_len);

        memset(mqtt_client->mqtt_data_buf, 0, mqtt_client->mqtt_data_buf_len);
        /* 回复报文超时判断 */
        if (mqtt_wait_ack(mqtt_client, CONNACK, &htim2, 200) != MQTT_SUCCESS) {
            mqtt_connect_flag = 1;
            printf("wait mqtt reconnect.\r\n");
        }
    }
#ifdef INFO_OUTPUT
    if (!mqtt_connect_flag) {
        printf("MQTT connected.\r\n");
    }
#endif
}

void MQTT_Disconnect(mqtt_client_t* mqtt_client)
{
    memset(mqtt_client->mqtt_data_buf, 0, mqtt_client->mqtt_data_buf_len);
    send_buf_len = MQTTSerialize_disconnect(mqtt_client->mqtt_data_buf, mqtt_client->mqtt_data_buf_len);
    send(mqtt_client->mqtt_sn, mqtt_client->mqtt_data_buf, send_buf_len);
}

void MQTT_Keepalive(mqtt_client_t* mqtt_client)
{
    // memset(mqtt_client->mqtt_data_buf, 0, mqtt_client->mqtt_data_buf_len);
    // make_ping_msg(mqtt_client->mqtt_data_buf, mqtt_client->mqtt_data_buf_len);
    // send(mqtt_client->mqtt_sn, mqtt_client->mqtt_data_buf, mqtt_client->mqtt_data_buf_len);
    // mqtt_connect_time = 0;
}

void MQTT_Subscribe(mqtt_client_t* mqtt_client, const char* topic)
{
    memset(mqtt_client->mqtt_data_buf, 0, mqtt_client->mqtt_data_buf_len);

    /*MQTT拼接订阅报文*/
    send_buf_len = make_sub_msg((char*)topic, mqtt_client->mqtt_data_buf, mqtt_client->mqtt_data_buf_len);
    send(mqtt_client->mqtt_sn, mqtt_client->mqtt_data_buf, send_buf_len);

    if (mqtt_wait_ack(mqtt_client, SUBACK, &htim2, 200) != MQTT_SUCCESS) {
        mqtt_subscribe_flag = 1;
        HAL_Delay(1000);
    }
    else {
        mqtt_subscribe_flag = 0;
        printf("Topic:%s subscribe done\n",topic);
    }
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
        memset(mqtt_client->mqtt_data_buf, 0, mqtt_client->mqtt_data_buf_len);
        recv(mqtt_client->mqtt_sn, mqtt_client->mqtt_data_buf, len);
        if (PUBLISH == mqtt_decode_msg(mqtt_client->mqtt_data_buf)) {
            printf("publish\r\n");
            MQTTDeserialize_publish(&dup, &qos, &retained, &(mqtt_client->mqtt_packet_id), &receivedTopic, &payload, &payloadlen, mqtt_client->mqtt_data_buf, mqtt_client->mqtt_data_buf_len);

            memset(recv_topic, 0, sizeof(recv_topic));
            memcpy(recv_topic, receivedTopic.lenstring.data, receivedTopic.lenstring.len);
            printf("topic: %s, message arrived %d: %s\n\r", recv_topic, payloadlen, payload);
        }
        else if (PINGRESP == mqtt_decode_msg(mqtt_client->mqtt_data_buf)) {
            printf("pingresp\r\n");
        }
        else {
            printf("wait publish\r\n");
        }
    }
}

void MQTT_ClientDaemon(mqtt_client_t* mqtt_client)
{
    MQTT_Reconnect(mqtt_client);
    if (1 == mqtt_subscribe_flag) {
        MQTT_Subscribe(mqtt_client, (const char*)sub_topic);
    }
    // if(mqtt_connect_time > 10000){}

    MQTT_Receivehandle(mqtt_client);
}