#include "at.h"
#include <stddef.h>
#include <string.h>


const char* AT_Handle_test(unsigned char *p, unsigned char len)
{
    return AT_CMD_OK;
}

const char* AT_Computer_PowerON(unsigned char* p, unsigned char len)
{}

const char* AT_Computer_PowerOFF(unsigned char* p, unsigned char len)
{}

/* AT指令表 */
const AT_CmdTypeDef at_cmd_index[] = {
{AT_CMD_TEST,         "AT",                AT_Handle_test},
{AT_CMD_TX_POWER,     "AT+PCPWRON=",       AT_Computer_PowerON},
{AT_CMD_TX_POWER,     "AT+PCPWROFF=",      AT_Computer_PowerOFF},
{AT_END,              NULL,                NULL}
};

/* 查找指令表中对应的指令 */
unsigned char AT_CMD_Search(unsigned char *p, unsigned char len)
{
    unsigned char cmdidx = 0;
    unsigned char i, n;

    for(i=1; at_cmd_index[i].cmd != AT_END; i++)
    {
        n = strlen(at_cmd_index[i].str);
        if(!strncmp((const char *)p, at_cmd_index[i].str, n)){
            cmdidx = i;
            break;
        }
    }
    return cmdidx;
}

/* AT指令解析 */
const char* AT_CMD_Parse(unsigned char *p, unsigned int len)
{
    unsigned char index = 0;

    /* 不符合指令最小长度 */
    if(len < 2){
        return AT_CMD_ERR;
    }
    /* AT格式错误*/
    if((p[0] != 'A') || (p[1] != 'T')){
        return AT_CMD_ERR;
    }
    /* 测试指令 */
    if(len == 2) {
        at_cmd_index[AT_CMD_TEST].handle(NULL, 0);
        return AT_CMD_OK;
    }
    /* 指令格式错误 */
    if(p[2] != '+'){
        return AT_CMD_ERR;
    }
    /* 查找其余匹配的执行指令，0-已匹配，!0-未匹配 */
    index = AT_CMD_Search(p, len);
    if(!index){
        return AT_CMD_ERR_UNINVAIL;         /* 未找到匹配的指令 */
    }
    if(at_cmd_index[index].handle == NULL){
        return AT_CMD_ERR_FUN_UNUSED;       /* 没有可执行函数 */
    }
    unsigned char n;
    n = strlen(at_cmd_index[index].str);
    return at_cmd_index[index].handle(p+n, len-n); /* 执行对应的指令函数, p+n:将指令参数传输执行函数，len-n-2:指令参数有效长度 */
}
