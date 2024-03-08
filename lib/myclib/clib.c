#include "clib.h"

const uint8_t sht75_crc_table[] = {
    0,   49,  98,  83,  196, 245, 166, 151, 185, 136, 219, 234, 125, 76,  31,  46,
    67,  114, 33,  16,  135, 182, 229, 212, 250, 203, 152, 169, 62,  15,  92,  109,
    134, 183, 228, 213, 66,  115, 32,  17,  63,  14,  93,  108, 251, 202, 153, 168,
    197, 244, 167, 150, 1,   48,  99,  82,  124, 77,  30,  47,  184, 137, 218, 235,
    61,  12,  95,  110, 249, 200, 155, 170, 132, 181, 230, 215, 64,  113, 34,  19,
    126, 79,  28,  45,  186, 139, 216, 233, 199, 246, 165, 148, 3,   50,  97,  80,
    187, 138, 217, 232, 127, 78,  29,  44,  2,   51,  96,  81,  198, 247, 164, 149,
    248, 201, 154, 171, 60,  13,  94,  111, 65,  112, 35,  18,  133, 180, 231, 214,
    122, 75,  24,  41,  190, 143, 220, 237, 195, 242, 161, 144, 7,   54,  101, 84,
    57,  8,   91,  106, 253, 204, 159, 174, 128, 177, 226, 211, 68,  117, 38,  23,
    252, 205, 158, 175, 56,  9,   90,  107, 69,  116, 39,  22,  129, 176, 227, 210,
    191, 142, 221, 236, 123, 74,  25,  40,  6,   55,  100, 85,  194, 243, 160, 145,
    71,  118, 37,  20,  131, 178, 225, 208, 254, 207, 156, 173, 58,  11,  88,  105,
    4,   53,  102, 87,  192, 241, 162, 147, 189, 140, 223, 238, 121, 72,  27,  42,
    193, 240, 163, 146, 5,   52,  103, 86,  120, 73,  26,  43,  188, 141, 222, 239,
    130, 179, 224, 209, 70,  119, 36,  21,  59,  10,  89,  104, 255, 206, 157, 172
};

uint32_t clib_strlen(char *string)
{
    uint32_t len = 0;

    while(*string++ != '\0')
        ++len;
    return len;
}

uint8_t clib_memcmp(uint8_t *des, uint8_t *src, uint32_t size)
{
    while(size--)
    {
        if(*src++ != *des++)
            return 0;
    }
    return 1;
}

void clib_memcpy(uint8_t *des, uint8_t *src, uint32_t size, uint32_t offset)
{
    int32_t idx = 0;

    while(size--)
    {
        des[offset + idx] = src[idx];
        ++idx;
    }
}

void clib_memset(uint8_t *des, uint8_t data, uint32_t size)
{
    while(size--)
    {
        *des++ = data;
    }
}

uint8_t clib_crc_8(const uint8_t *input_str, uint32_t num_bytes)
{
    uint32_t a;
    uint8_t crc;
    const uint8_t *ptr;

    crc = 0x00;
    ptr = input_str;

    if ( ptr != 0 ) for (a=0; a<num_bytes; a++) {

        crc = sht75_crc_table[(*ptr++) ^ crc];
    }

    return crc;
}

uint8_t clib_update_crc_8(uint8_t crc, uint8_t val)
{
	return sht75_crc_table[val ^ crc];
}

/* end of clib.c */
