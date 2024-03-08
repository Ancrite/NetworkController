#include "flash.h"
#include "stm32f1xx_hal.h"

void BSP_FLASH_ReadData(uint32_t address, uint8_t* pbuf, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++){
        pbuf[i] = *(__IO uint8_t*)address;
        address ++;
    }
}

void BSP_FLASH_WriteData(uint32_t address, uint16_t* pbuf, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, pbuf[i]);
        address += 2;     /* 指向下一个半字 */
    }
}

uint8_t BSP_FLASH_WritePages(uint32_t address, uint8_t* pbuf, uint32_t length)
{
    /* 影子数组，用于保存扇区原始数据 */
    static uint8_t shadow_ram[FLASH_PAGESIZE];

    /* 扇区编号 */
    uint16_t page_number = (address - FLASH_STARTADDR) / FLASH_PAGESIZE;

    /* 扇区内偏移量(字节) */
    uint32_t page_offset = ((address - FLASH_STARTADDR) % FLASH_PAGESIZE);

    /* 扇区内剩余长度(字节) */
    uint32_t page_remain_length = FLASH_PAGESIZE - page_offset;

    /* HAL FLASH 擦除结构体*/
    FLASH_EraseInitTypeDef FLASH_EraseInitStruct;

    /* 擦除错误扇区地址 */
    uint32_t erase_erraddr;

    /* 验证地址范围 */
    if (address < FLASH_STARTADDR || (address >= (FLASH_STARTADDR + FLASH_SIZE))) {
        return 1;     // 非法地址
    }

    /* FLASH 写入前先解锁 */
    HAL_FLASH_Unlock();

    while (1) {
        /* 保存原始数据 */
        BSP_FLASH_ReadData(FLASH_STARTADDR + page_number * FLASH_PAGESIZE, shadow_ram, FLASH_PAGESIZE);

        /* 擦除扇区 */
        FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;    // 选择页擦除
        FLASH_EraseInitStruct.NbPages = 1;
        FLASH_EraseInitStruct.PageAddress = FLASH_STARTADDR + page_number * FLASH_PAGESIZE;  // 要擦除的扇区
        if (HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &erase_erraddr) != HAL_OK) {
            HAL_FLASH_Lock();
            return 2;
        }

        /* 修改并写入扇区 */
        for (uint32_t i = 0; i < page_remain_length; i++) {
            shadow_ram[page_offset + i] = pbuf[i];
        }
        BSP_FLASH_WriteData(FLASH_STARTADDR + page_number * FLASH_PAGESIZE, (uint16_t *)shadow_ram, FLASH_PAGESIZE / 2);

        /* 写入结束了 */
        if (length <= page_remain_length) {
            break;
        }
        /* 写入未结束 */
        else {
            page_number ++;                             /* 扇区地址增1 */
            page_offset = 0;                            /* 扇区偏移置0 */
            pbuf   += page_remain_length;               /* 数据指针偏移 */
            length -= page_remain_length;               /* 剩余数据递减 */
            page_remain_length = FLASH_PAGESIZE;
        }
    }

    /* FLASH 写入后加锁 */
    HAL_FLASH_Lock();

    return 0;
}

void BSP_FLASH_GetUserParameters(userparameter_t* info)
{
    BSP_FLASH_ReadData((FLASH_STARTADDR + USERPARAMETER_PAGE * FLASH_PAGESIZE),
                   (uint8_t*)info,
                   sizeof(userparameter_t));
}

uint8_t BSP_FLASH_SetUserParameters(userparameter_t* info)
{
    return BSP_FLASH_WritePages(FLASH_STARTADDR + USERPARAMETER_PAGE * FLASH_PAGE_SIZE,
                                (uint8_t*)info,
                                sizeof(userparameter_t));
}
/* end of flash.c */