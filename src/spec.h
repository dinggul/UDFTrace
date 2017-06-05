#pragma once
#include <stdint.h>

typedef enum {
    UDF_VOID = 0,
    UDF_CHAR = 1,
    UDF_I32  = 2,
    UDF_I64  = 3,
    UDF_U32  = 4,
    UDF_U64  = 5,
    UDF_STR  = 6,
    UDF_MEM  = 7,
} UDFTypeKind;

typedef enum {
    UDF_STACK = 0,
    UDF_RAX   = 1,
    UDF_RBX   = 2,
    UDF_RCX   = 3,
    UDF_RDX   = 4,
    UDF_RDI   = 5,
    UDF_RSI   = 6,
    UDF_RBP   = 7,
    UDF_RSP   = 8,
    UDF_R8    = 9,
    UDF_R9    = 10,
    UDF_R10   = 11,
    UDF_R11   = 12,
    UDF_R12   = 13,
    UDF_R13   = 14,
    UDF_R14   = 15,
    UDF_RS5   = 16,
} UDFTypeLoc;

typedef struct {
    uint8_t kind;   // argument type.
    uint8_t loc;    // argument location (stack or register).
    uint16_t ofs;   // used for location = stack arguments.
    uint32_t size;  // used for type = memory arguments.
} UDFType;

typedef struct {
    uint64_t addr;
    UDFType ret;
    UDFType args[6];   // Maximum 6 arguments.
} UDFFunc;

typedef struct {
    uint64_t nfuncs;
    UDFFunc funcs[0];    // 'nfuncs' elements.
} UDFSpec;
