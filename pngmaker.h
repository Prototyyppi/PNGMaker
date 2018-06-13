/*
 * Header for PNGMaker
 */

#ifndef __PNGMaker_H__
#define __PNGMaker_H__
#include <stdint.h>

/* Calculate crc for data */
int crc32();

/* Wrapper for crc calculation */
int calculate_crc(uint8_t* , int);

#endif /* __PNGMaker_H__ */