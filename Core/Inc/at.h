#ifndef __AT_H
#define __AT_H

#include "main.h"

#define AT_CMD_OK               ("0") /* 指令正常 */
#define AT_CMD_ERR              ("1") /* 指令异常 */
#define AT_CMD_ERR_UNINVAIL     ("2") /* 没有对应指令 */
#define AT_CMD_ERR_FUN_UNUSED   ("3") /* 没有可执行函数 */

#define AT_HANDLE_OK            ("OK")
#define AT_HANDLE_ERR_DATA      ("ERR_DATA") /* 数据错误 */
#define AT_HANDLE_ERR_          ()

typedef enum
{
    AT_CMD_TEST = 0,     /* 测试指令 */
    AT_CMD_TX_POWER,     /* 设置发送功率 */

    AT_END
}AT_Command;

typedef const char* (*ATCallback)(unsigned char* ptr, unsigned char len);

typedef struct
{
    AT_Command  cmd;     /* 指令序号 */
    const char* str;     /* 指令内容 */
    ATCallback  handle;  /* 指令执行 */
}AT_CmdTypeDef;

extern uint8_t at_buf[100];

const char* AT_CMD_Parse(unsigned char* p, unsigned int len);

#endif
