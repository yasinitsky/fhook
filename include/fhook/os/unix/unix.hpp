/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains all OS-dependent types, function and constants for Unix (Linux and some BSD's).
*/

#if (defined __linux__ || defined __OpenBSD__ || defined __NetBSD__ || defined __FreeBSD__) && !defined __ANDROID__
#define FHOOK_OS_INCLUDED

#ifndef FHOOK_OS_UNIX_HPP
#define FHOOK_OS_UNIX_HPP

#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdint.h>

namespace fhook
{
    typedef void* MemoryAllocationResult;
    typedef int MemoryProtectionResult;
    typedef int MemoryProtectionFlags;

    const MemoryProtectionFlags MEMORY_PROTECTION_ALL = PROT_READ | PROT_WRITE | PROT_EXEC;
    const MemoryProtectionFlags MEMORY_PROTECTION_DEFAULT = PROT_EXEC | PROT_READ;

    typedef int ErrorCode;

    inline bool memoryAllocationSuccess(MemoryAllocationResult result)
    {
        return (result != MAP_FAILED);
    }

    inline bool memoryProtectionSuccess(MemoryProtectionResult result)
    {
        return (result == 0);
    }

    inline ErrorCode getErrorCode()
    {
        return errno;
    }

    MemoryAllocationResult allocateMemory(size_t length)
    {
        MemoryAllocationResult mem = mmap(NULL, length, MEMORY_PROTECTION_ALL, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        return mem;
    }

    MemoryProtectionResult protectMemory(VoidPointer address, size_t length, MemoryProtectionFlags protection)
    {
        long pageSize = sysconf(_SC_PAGESIZE);

        if(pageSize == -1) return 0;

        VoidPointer alignedAddress = (VoidPointer)( (uint64_t)address & ~(pageSize-1) );
        size_t newLength = ( (size_t)address + length ) - (size_t)alignedAddress;

        return mprotect(alignedAddress, newLength, protection);
    }
}


#endif // FHOOK_OS_UNIX_HPP

#endif // OS-specific macros