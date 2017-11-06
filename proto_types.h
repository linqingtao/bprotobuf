#ifndef __PROTO_TYPES_H__
#define __PROTO_TYPES_H__
/*#if !defined(ARM)
#include <stdint.h>
#else
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif*/

enum {
    OBJECT_INT8,
    OBJECT_UINT8,
    OBJECT_INT16,
    OBJECT_UINT16,
    OBJECT_INT32,
    OBJECT_UINT32,
    OBJECT_INT64,
    OBJECT_UINT64,
    OBJECT_FLOAT,
    OBJECT_DOUBLE,
    OBJECT_STRING,
    OBJECT_BITS,
    OBJECT_BYTES,
    OBJECT_ARRAY,
    OBJECT_MESSAGE,
    OBJECT_PROTOCOL,
};

enum {
    PARSE_OK,
    PARSE_FILE_OPEN_ERROR,
    PARSE_INVALID_FILE,
};



/*
struct tagArray;

typedef struct tagArray {
    union {
        void* ptr;
        int8_t _int8;
        uint8_t _uint8;
        int16_t _int16;
        uint16_t _uint16;
        int32_t  _int32;
        uint32_t _uint32;
        int64_t _int64;
        uint64_t _uint64;
        float _float;
        double _double;
    };
    struct tagArray* next;
} Array;*/

//extern int g_small_endian;

#endif

