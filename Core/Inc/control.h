#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

#define COMPUTER   1
#define PROJRCTOR  0

#if COMPUTER

#define PCPORT 0b11111111
#define PCPORTCOUNT  8

typedef struct
{
    uint8_t relay_number;           //端口编号
    uint8_t command;                //端口需要执行的指令
    uint8_t port_status;            //端口当前状态
    uint8_t exec_result;            //端口指令执行结果
    uint32_t relay_PULLIN_tim;      //端口继电器吸合时基计数
}Computer_Port_t;

#define POWER_PC_TIM   1000  //继电器动作间隔时间  1s
#define GAP_PC_TIM     1000  //相邻继电器指令间隔时间  1s
#define FORCE_OFF_TIM  6000  //强制关机指令继电器动作间隔时间  6s

#define CMD_NULL        0x30 //空指令标记
#define CMD_ON          0x31 //开机指令标记
#define CMD_OFF         0x32 //关机指令标记
#define CMD_FORCE_OFF   0x34 //强制关机指令标记
#define CMD_QUERY       0x38 //查询指令标记

#define PORT_POWERON        0x01
#define PORT_POWEROFF       0x02
#define PORT_RELAYPULLIN    0x04

#define EXEC_SUCCESS         0x01       //成功
#define EXEC_UNREADY         0x02       //未准备
#define EXEC_READY           0x04       //准备
#define EXEC_AFTER_ON        0x08       //开指令执行中
#define EXEC_AFTER_OFF       0x10       //关指令执行中
#define EXEC_AFTER_FORCEOFF  0x20       //强制关指令执行中

#endif

#if PROJRCTOR
typedef struct
{
    uint8_t number;             //端口编号
    uint8_t command;            //端口需要执行的指令
    uint8_t cmd_flag;           //端口是否正在执行指令标志
    uint8_t return_cmd_flag;    //返回指令类型
    uint8_t power_status;       //端口开关状态
    uint8_t exec_result;        //端口指令执行结果
    uint8_t projector_brand;    //端口对应的投影机品牌型号
}Projector_Port_t;

static uint8_t  projector_on[] = { 0x02,0x00,0x00,0x00,0x00,0x02 };                     //开机
static uint8_t  projector_off[] = { 0x02,0x01,0x00,0x00,0x00,0x03 };                    //关机
static uint8_t  projector_time[] = { 0x43,0x52,0x33,0x0d,0x0a,0x0d,0x0a,0x0d,0x0a };    //时间
static uint8_t  projector_temp[] = { 0x43,0x52,0x36,0x0d,0x0a,0x0d,0x0a,0x0d,0x0a };    //温度

#define projector_NEC 0
static uint8_t projector_on_NEC[] = { 0x02,0x00,0x00,0x00,0x00,0x02 };                                                      //NEC开机    波特率：38400
static uint8_t projector_off_NEC[] = { 0x02,0x01,0x00,0x00,0x00,0x03 };                                                     //NEC关机

#define projector_SX 1
static uint8_t  projector_on_SX[] = { 0x02,0x41,0x44,0x5A,0x5A,0x3B,0x50,0x4F,0x4E,0x03,0x0d,0x0a };                        //松下开机    波特率：9600
static uint8_t  projector_off_SX[] = { 0x02,0x41,0x44,0x5A,0x5A,0x3B,0x50,0x4F,0x46,0x03,0x0d,0x0a };                       //松下关机
static uint8_t  projector_time_SX[] = { 0x02,0x41,0x44,0x5A,0x5A,0x3B,0x51,0x24,0x4c,0x3a,0x31,0x03,0x0d,0x0a };            //松下时间

#define projector_SX_2 9
static uint8_t  projector_on_SX_2[] = { 0x02,0x41,0x44,0x5A,0x5A,0x3B,0x50,0x4F,0x4E,0x03 };                                //松下开机    波特率：19200
static uint8_t  projector_off_SX_2[] = { 0x02,0x41,0x44,0x5A,0x5A,0x3B,0x50,0x4F,0x46,0x03 };                               //松下关机

#define projector_MJ 2
static uint8_t  projector_on_MJ[] = { 0x5a,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x5a };    //明基开机    波特率：115200
static uint8_t  projector_off_MJ[] = { 0x5a,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5a };   //明基关机

//static char  projector_on_MJ[] = {0x0D,0x2A,0x70,0x6F,0x77,0x3D,0x6F,0x6E,0x23,0x0D};                                     //明基开机    波特率：115200  <CR>*pow=on#<CR>
//static char  projector_off_MJ[]= {0x0D,0x2A,0x70,0x6F,0x77,0x3D,0x6F,0x66,0x66,0x23,0x0D};                                //明基关机                   <CR>*pow=off#<CR>

#define projector_BK 3
static uint8_t  projector_on_BK[] = { 0xfe,0x00,0x58,0x03,0x5b,0xff };                                                      //巴克开机    波特率：9600
static uint8_t  projector_off_BK[] = { 0xfe,0x00,0x58,0x00,0x58,0xff };                                                     //巴克关机

#define projector_XXX 4
static uint8_t  projector_on_XXX[] = { 0x06,0x14,0x00,0x04,0x00,0x34,0x11,0x00,0x00,0x5D };                                 //未知品牌开机    波特率：?
static uint8_t  projector_off_XXX[] = { 0x06,0x14,0x00,0x04,0x00,0x34,0x11,0x01,0x00,0x5E };                                //未知品牌关机

#define projector_ABC 5
static uint8_t  projector_on_ABC[] = { 0x23,0x50,0x57,0x52,0x30,0x2c,0x31,0x21 };                                           //信阳未知品牌开机    波特率：19200
static uint8_t  projector_off_ABC[] = { 0x23,0x50,0x57,0x52,0x30,0x2c,0x30,0x21 };                                          //信阳未知品牌关机

#define projector_WJ 6
static uint8_t  projector_on_WJ[] = { 0xBE,0xEF,0x03,0x06,0x00,0xBA,0xD2,0x01,0x00,0x00,0x60,0x01,0x00 };                   //王甲未知品牌开机    波特率：19200
static uint8_t  projector_off_WJ[] = { 0xBE,0xEF,0x03,0x06,0x00,0x2A,0xD3,0x01,0x00,0x00,0x60,0x00,0x00 };                  //王甲未知品牌关机    日立

#define projector_VP 7
static uint8_t  projector_on_VP[] = { 0x50,0x57,0x52,0x20,0x4F,0x4E,0x0D };                                                 //王甲VP21开机     波特率：9600
static uint8_t  projector_off_VP[] = { 0x50,0x57,0x52,0x20,0x4F,0x46,0x46,0x0D };                                           //王甲VP21关机

#define projector_SML 8
static uint8_t  projector_on_SML[] = { 0x23,0x50,0x57,0x52,0x30,0x2C,0x31,0x21 };                                           //视美乐开机    波特率：19200
static uint8_t  projector_off_SML[] = { 0x23,0x50,0x57,0x52,0x30,0x2C,0x30,0x21 };                                          //视美乐关机

#define projector_SML_2 11
static uint8_t  projector_on_SML_2[] = { 0x23,0x50,0x57,0x52,0x31,0x21 };                                                   //山东滕州开机    波特率：19200   #PWR1！
static uint8_t  projector_off_SML_2[] = { 0x23,0x50,0x57,0x52,0x30,0x21 };                                                  //山东滕州关机    合肥中院        #PWR0！

#define projector_AQ 10
static uint8_t  projector_on_AQ[] = { 0x43,0x30,0x30,0x0d,0x0a,0x0d,0x0a,0x0d,0x0a };                                       //爱其开机    波特率：19200
static uint8_t  projector_off_AQ[] = { 0x43,0x30,0x31,0x0d,0x0a,0x0d,0x0a,0x0d,0x0a };                                      //爱其关机

#define projector_SONY 12
static uint8_t  projector_on_SONY[] = { 0xA9,0x17,0x2E,0x00,0x00,0x00,0x3F,0x9A };                                          //虾蟹馆索尼开机    波特率：38400
static uint8_t  projector_off_SONY[] = { 0xA9,0x17,0x2F,0x00,0x00,0x00,0x3F,0x9A };                                         //虾蟹馆索尼关机    数据位8位 停止位1位 偶校验

#define projector_SONY_2 15
static uint8_t  projector_on_SONY_2[] = { 0xA9,0x01,0x30,0x00,0x00,0x01,0x31,0x9A };                                        //华为5G馆索尼开机    波特率：38400
static uint8_t  projector_off_SONY_2[] = { 0xA9,0x01,0x30,0x00,0x00,0x00,0x31,0x9A };                                       //华为5G馆索尼关机    数据位8位 停止位1位 偶校验

#define projector_Epson 13
static uint8_t  projector_on_Epson[] = { 0x50,0x57,0x52,0x20,0x4F,0x4E,0x0D };                                              //爱普生开机    波特率：9600
static uint8_t  projector_off_Epson[] = { 0x50,0x57,0x52,0x20,0x4F,0x46,0x46,0x0D };                                        //爱普生关机

#define projector_Optoma 14
static uint8_t  projector_on_Optoma[] = { 0x7E,0x30,0x30,0x30,0x30,0x20,0x31,0x0D };                                        //奥图码开机    波特率：9600
static uint8_t  projector_off_Optoma[] = { 0x7E,0x30,0x30,0x30,0x30,0x20,0x30,0x0D };                                       //奥图码关机

#define projector_acer 15
static uint8_t  projector_on_ACER[] = { 0x50,0x4F,0x57,0x52,0x20,0x20,0x20,0x31,0x0D };                                     //acer开机    波特率：9600
static uint8_t  projector_off_ACER[] = { 0x50,0x4F,0x57,0x52,0x20,0x20,0x20,0x30,0x0D };                                    //acer关机    龙袍 宏基

#define projector_NECC 16
static uint8_t projector_on_NECC[] = { 0x23,0x30,0x30,0x30,0x30,0x20,0x31,0x0D };                                           //NEC开机    波特率：19200
static uint8_t projector_off_NECC[] = { 0x23,0x30,0x30,0x30,0x30,0x20,0x30,0x0D };                                          //NEC关机

#define projector_SNK 17
static uint8_t projector_on_SNK[] = { 0x23,0x30,0x30,0x30,0x30,0x20,0x31,0x0D };                                            //索诺克开机    波特率：19200
static uint8_t projector_off_SNK[] = { 0x23,0x30,0x30,0x30,0x30,0x20,0x30,0x0D };                                           //索诺克关机

#define projector_SNP 18
static uint8_t projector_on_SNP[] = { 0x5B,0x50,0x4F,0x57,0x52,0x31,0x5D };                                                 //SNP开机    波特率：19200
static uint8_t projector_off_SNP[] = { 0x5B,0x50,0x4F,0x57,0x52,0x30,0x5D };                                                //SNP关机

#define projector_RL 19
static uint8_t projector_on_RL[] = { 0xBE,0xEF,0x03,0x06,0x00,0xBA,0xD2,0x01,0x00,0x00,0x60,0x01,0x00 };                    //日立开机    波特率：19200
static uint8_t projector_off_RL[] = { 0xBE,0xEF,0x03,0x06,0x00,0x2A,0xD3,0x01,0x00,0x00,0x60,0x00,0x00 };                   //日立关机

#define projector_MAXELL 20
static uint8_t projector_on_MAXELL[] = { 0xBE,0xEF,0x03,0x06,0x00,0xBA,0xD2,0x01,0x00,0x00,0x60,0x01,0x00 };                //MAXELL开机    波特率：19200
static uint8_t projector_off_MAXELL[] = { 0xBE,0xEF,0x03,0x06,0x00,0x2A,0xD3,0x01,0x00,0x00,0x60,0x00,0x00 };               //MAXELL关机

#define projector_LIXUN 21
static uint8_t projector_on_LIXUN[] = { 0x4F,0x20,0x50,0x4F,0x57,0x45,0x52,0x2E,0x4F,0x4E,0x0D };                           //LIXUN开机    波特率：9600
static uint8_t projector_off_LIXUN[] = { 0x4F,0x20,0x50,0x4F,0x57,0x45,0x52,0x2E,0x4F,0x46,0x46,0x0D };                     //LIXUN关机

#define Sonnoc 22
static uint8_t Sonnoc_on[] = { 0x23,0x30,0x30,0x30,0x30,0x20,0x31,0x0D };                                                   //LIXUN开机    波特率：19200
static uint8_t Sonnoc_off[] = { 0x23,0x30,0x30,0x30,0x30,0x20,0x30,0x0D };                                                  //LIXUN关机


#define ReturnPOW_command_len 12       //POW返回指令长度
#define ReturnPOW_commandtitle_len 4   //POW返回指令头长度
#define ReturnCMD_command_len 12       //CMD返回指令长度
#define ReturnCMD_commandtitle_len 4   //CMD返回指令头长度

#define POWER_ON           0x31    //处于开机状态
#define POWER_OFF          0x30    //处于关机状态

#define CMD_NULL           0x30    //空指令标记
#define CMD_ON             0x31    //开机指令标记
#define CMD_OFF            0x32    //关机指令标记
#define CMD_QUERY          0x34    //查询指令标记

#define CMD_executing      0x01    //指令执行过程中
#define CMD_finished       0x00    //指令执行完成

#define Return_NULL        0x00    //不返回任何指令
#define Return_POW         0x01    //POW返回指令标记
#define Return_CMD         0x02    //CMD返回指令标记
#define Return_on          0x03    //开机返回指令标记
#define Return_off         0x04    //关机返回指令标记
#define Return_query       0x05    //查询返回指令标记

#define exec_SUCCESS       0x01    //成功
#define exec_UNREADY       0x00    //未准备
#define exec_READY         0x05    //准备
#define exec_AFTER_ON      0x02    //开指令执行中
#define exec_AFTER_OFF     0x03    //关指令执行中

#define projector_one      0x01    //品牌1
#define projector_two      0x02    //品牌2
#define projector_three    0x03    //品牌3
#define projector_four     0x04    //品牌4

#define USART1_on_data_len    12    //串口1开指令长度
#define USART2_on_data_len    12    //串口2开指令长度
#define USART3_on_data_len    12    //串口3开指令长度
#define USART4_on_data_len    12    //串口4开指令长度

#define USART1_off_data_len   12    //串口1关指令长度
#define USART2_off_data_len   12    //串口2关指令长度
#define USART3_off_data_len   12    //串口3关指令长度
#define USART4_off_data_len   12    //串口4关指令长度

#define USART1_on_data projector_on_SX    //串口1开指令
#define USART2_on_data projector_on_SX    //串口2开指令
#define USART3_on_data projector_on_SX    //串口3开指令
#define USART4_on_data projector_on_SX    //串口4开指令

#define USART1_off_data projector_off_SX    //串口1关指令
#define USART2_off_data projector_off_SX    //串口2关指令
#define USART3_off_data projector_off_SX    //串口3关指令
#define USART4_off_data projector_off_SX    //串口4关指令

#endif

void Computer_PowerChange(void);
void Projector_PowerChange(void);

#endif