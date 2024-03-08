#ifndef __CLIB_H
#define __CLIB_H

#include <stdint.h>

uint32_t clib_strlen(char *string);
uint8_t  clib_memcmp(uint8_t* des, uint8_t* src, uint32_t size);
void     clib_memcpy(uint8_t* des, uint8_t* src, uint32_t size, uint32_t offset);
void     clib_memset(uint8_t* des, uint8_t data, uint32_t size);
uint8_t  clib_crc_8(const uint8_t *input_str, uint32_t num_bytes);
uint8_t  clib_update_crc_8(uint8_t crc, uint8_t val);

#endif /* end of __CLIB_H */