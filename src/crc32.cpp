#include "crc32.h"

const uint32_t Polynomial = 0xEDB88320;

uint32_t crc32_bitwise(const void* data, size_t length, uint32_t previousCrc32)
{
	uint32_t crc = ~previousCrc32; // same as previousCrc32 ^ 0xFFFFFFFF
	unsigned char* current = (unsigned char*) data;
	for (int i = 0; i < length; i++)
	{
		crc ^= current[i];
		for (unsigned int j = 0; j < 8; j++){
			if (crc & 1)
				crc = (crc >> 1) ^ Polynomial;
			else
				crc =  crc >> 1;
		}
	}
	return ~crc; // same as crc ^ 0xFFFFFFFF
}

