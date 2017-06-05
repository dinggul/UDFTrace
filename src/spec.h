#pragma once
#include <stdint.h>

typedef struct {
    uint8_t kind;       // argument type.
    uint8_t location;   // argument location (stack or register).
    uint16_t stack_ofs; // used for stack arguments.
    uint32_t mem_size;  // used for memory arguments.
} UDFType;

typedef struct {
    uint64_t addr;
    uint64_t nargs;
    UDFType ret_ty;
    UDFType arg_ty[0]; // 'nargs' elements.
} UDFFunc;

typedef struct {
    uint64_t nfuncs;
    UDFFunc funcs[0]; // 'nfuncs' elements.
} UDFSpec;
