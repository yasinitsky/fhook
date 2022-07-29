/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains all architecture-dependent types and functions for AMD64 (x86_64).
*/

#if defined __amd64__ || defined _M_AMD64
#define FHOOK_ARCH_INCLUDED

#ifndef FHOOK_ARCH_AMD64_HPP
#define FHOOK_ARCH_AMD64_HPP

#if defined _MSC_VER
    typedef unsigned __int8 uint8_t;
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t;
    typedef __int64 int64_t;
#else
    #include <stdint.h>
#endif

namespace fhook
{
    typedef int64_t LongestInteger;

    #pragma pack(push, 1)
    typedef struct
    {
        uint8_t push = 0x68;                // push
        uint32_t lowerBytes = 0x00000000; 
        uint8_t movOpcode = 0xc7;
        uint8_t movModRM = 0x44;
        uint8_t movSIB = 0x24;
        uint8_t movOffset = 0x04;           // mov [rsp+4]
        uint32_t upperBytes = 0x00000000;
        uint8_t ret = 0xc3;                 // ret
    } Jump;
    #pragma pack(pop)

    #pragma pack(push, 1)
    typedef struct
    {
        uint8_t oldCode[15 + sizeof(Jump)];
        Jump jmp;
    } Trampoline;
    #pragma pack(pop)

    Jump makeJump(VoidPointer address)
    {
        Jump bytecode;

        bytecode.lowerBytes = ((uint32_t*) &address)[0];
        bytecode.upperBytes = ((uint32_t*) &address)[1];

        return bytecode;
    }

    Trampoline makeTrampoline(VoidPointer address)
    {
        Trampoline bytecode;

        for(int i = 0; i < sizeof(bytecode.oldCode)/sizeof(bytecode.oldCode[0]); i++)
        {
            bytecode.oldCode[i] = 0x90; // nop
        }

        bytecode.jmp = makeJump(address);

        return bytecode;
    }

}

#endif // FHOOK_ARCH_AMD64_HPP

#endif // CPU-specific macros