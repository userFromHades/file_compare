#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

uint32_t crc32_bitwise(const void* data, size_t length, uint32_t previousCrc32 = 0);

#endif //CRC32_H
