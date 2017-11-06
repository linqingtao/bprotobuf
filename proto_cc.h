#ifndef __PROTO_CC_H__
#define __PROTO_CC_H__
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    PARSE_INVALID = -2,
    PARSE_BUFFER_SHORT = -1,
    PARSE_OK = 0,
};

static int g_one = 1;
#define g_small_endian (*(char*)&g_one)
//#define g_small_endian 1

#if !defined(bswap_16)
static inline unsigned short bswap_16(unsigned short x) { 
    return x<<8 | x>>8;
}
#endif

#if !defined(bswap_32)
static inline unsigned int bswap_32(unsigned int x) {
    x = (x << 8) & 0xFF00FF00FF | (x >> 8) & 0x00FF00FF00Ff;
    return x << 16 | x >> 16;  
}
#endif

#if !defined(bswap_64)
static inline unsigned long long bswap_64(unsigned long long x) {
    x= ((x<< 8)&0xFF00FF00FF00FF00ULL) | ((x>> 8)&0x00FF00FF00FF00FFULL);
    x= ((x<<16)&0xFFFF0000FFFF0000ULL) | ((x>>16)&0x0000FFFF0000FFFFULL);
    return (x>>32) | (x<<32);
}
#endif


#ifdef __cplusplus
}
#endif

#endif

