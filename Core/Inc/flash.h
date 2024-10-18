#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"
#include "network.h"

#define FLASH_STARTADDR         0x08000000U          /* ARM Cortex 架构中 FLASH 的起始地址 */
#define FLASH_SIZE              0x80000U             /* MCU FLASH 总大小 */

#define USERPARAMETER_PAGE      50

/**
 * @brief 读取 FLASH 数据
 *
 * @note 可单字节或多字节读取
 * @param address 起始地址
 * @param pbuf    数据存储位置指针
 * @param length  数据长度(字节)
 */
void BSP_FLASH_ReadData(uint32_t address, uint8_t* pbuf, uint32_t length);

/**
 * @brief 按半字写入 FLASH 扇区
 *
 * @note 基本写入，无擦除操作，需要手动把原来的扇区擦除后再调用此函数写入
 * @param address 起始地址 (必须为2的倍数)
 * @param pbuf    要写入的数据首地址
 * @param length  数据长度(半字)
 */
void BSP_FLASH_WriteData(uint32_t address, uint16_t* pbuf, uint16_t length);

/**
 * @brief 将任意长度数据写入 FLASH 扇区
 *
 * @note 支持单字节写入，包含擦除和保留无修改部分的操作
 * @param address 起始地址
 * @param pbuf    要写入的数据首地址
 * @param length  数据长度(字节)
 * @return uint8_t
 */
uint8_t BSP_FLASH_WritePages(uint32_t address, uint8_t* pbuf, uint32_t length);

/**
 * @brief 从 FLASH 中获取用户配置参数
 *
 * @param info 用户配置结构体变量，用于保存读取的用户配置信息
 */
void BSP_FLASH_GetUserParameters(userparameter_t* info);

/**
 * @brief 向 FLASH 中写入用户配置参数
 *
 * @param info 用户配置结构体变量，需要写入的用户配置信息
 * @return uint8_t 结果标志
 */
uint8_t BSP_FLASH_SetUserParameters(userparameter_t* info);

#endif /* end of __FLASH_H */
