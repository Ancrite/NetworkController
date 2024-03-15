#ifndef  __MQTT_H
#define  __MQTT_H

#include "net.h"

#define MQTTSOC SOC_TCPS

typedef struct mqtt_client
{
    char*       mqtt_clientid;
    char*       mqtt_username;
    char*       mqtt_password;
    uint8_t*    mqtt_host;
    uint16_t    mqtt_port;
    uint8_t     mqtt_sn;
    void*       mqtt_reconnect_data;
    uint8_t*    mqtt_read_buf;
    uint8_t*    mqtt_write_buf;
    uint16_t    mqtt_keepalive_interval;
    uint16_t    mqtt_packet_id;
    uint32_t    mqtt_will_flag : 1;
    uint32_t    mqtt_cleansession : 1;
    uint32_t    mqtt_ping_outstanding : 2;
    uint32_t    mqtt_version : 4;
    uint32_t    mqtt_ack_handler_number : 24;
    uint32_t    mqtt_cmd_timeout;
    uint32_t    mqtt_reconnect_try_duration;
} mqtt_client_t;

typedef enum mqtt_error
{
    MQTT_SSL_CERT_ERROR                             = -0x001C,      /* cetr parse failed */
    MQTT_SOCKET_FAILED_ERROR                        = -0x001B,      /* socket fd failed */
    MQTT_SOCKET_UNKNOWN_HOST_ERROR                  = -0x001A,      /* socket unknown host ip or domain */
    MQTT_SET_PUBLISH_DUP_FAILED_ERROR               = -0x0019,      /* mqtt publish packet set udp bit failed */
    MQTT_CLEAN_SESSION_ERROR                        = -0x0018,      /* mqtt clean session error */
    MQTT_ACK_NODE_IS_EXIST_ERROR                    = -0x0017,      /* mqtt ack list is exist ack node */
    MQTT_ACK_HANDLER_NUM_TOO_MUCH_ERROR             = -0x0016,      /* mqtt ack handler number is too much */
    MQTT_RESUBSCRIBE_ERROR                          = -0x0015,      /* mqtt resubscribe error */
    MQTT_SUBSCRIBE_ERROR                            = -0x0014,      /* mqtt subscribe error */
    MQTT_SEND_PACKET_ERROR                          = -0x0013,      /* mqtt send a packet */
    MQTT_SERIALIZE_PUBLISH_ACK_PACKET_ERROR         = -0x0012,      /* mqtt serialize publish ack packet error */
    MQTT_PUBLISH_PACKET_ERROR                       = -0x0011,      /* mqtt publish packet error */
    MQTT_RECONNECT_TIMEOUT_ERROR                    = -0x0010,      /* mqtt try reconnect, but timeout */
    MQTT_SUBSCRIBE_NOT_ACK_ERROR                    = -0x000F,      /* mqtt subscribe, but not ack */
    MQTT_NOT_CONNECT_ERROR                          = -0x000E,      /* mqtt not connect */
    MQTT_SUBSCRIBE_ACK_PACKET_ERROR                 = -0x000D,      /* mqtt subscribe, but ack packet error */
    MQTT_UNSUBSCRIBE_ACK_PACKET_ERROR               = -0x000C,      /* mqtt unsubscribe, but ack packet error */
    MQTT_PUBLISH_ACK_PACKET_ERROR                   = -0x000B,      /* mqtt pubilsh ack packet error */
    MQTT_PUBLISH_ACK_TYPE_ERROR                     = -0x000A,      /* mqtt pubilsh ack type error */
    MQTT_PUBREC_PACKET_ERROR                        = -0x0009,      /* mqtt pubrec packet error */
    MQTT_BUFFER_TOO_SHORT_ERROR                     = -0x0008,      /* mqtt buffer too short */
    MQTT_NOTHING_TO_READ_ERROR                      = -0x0007,      /* mqtt nothing to read */
    MQTT_SUBSCRIBE_QOS_ERROR                        = -0x0006,      /* mqtt subsrcibe qos error */
    MQTT_BUFFER_OVERFLOW_ERROR                      = -0x0005,      /* mqtt buffer overflow */
    MQTT_CONNECT_FAILED_ERROR                       = -0x0004,      /* mqtt connect failed */
    MQTT_MEM_NOT_ENOUGH_ERROR                       = -0x0003,      /* mqtt memory not enough */
    MQTT_NULL_VALUE_ERROR                           = -0x0002,      /* mqtt value is null */
    MQTT_FAILED_ERROR                               = -0x0001,      /* failed */
    MQTT_SUCCESS                                    =  0x0000       /* success */
} mqtt_flag_t;

extern uint8_t server_ip[4];
extern uint8_t buffer[2048];
extern uint8_t msgbuf[1024];


#endif