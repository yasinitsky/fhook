/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains the implementation of OS-dependent methods for Unix (Linux, OpenBSD, NetBSD, FreeBSD).
*/

#if (defined __linux__ || defined __OpenBSD__ || defined __NetBSD__ || defined __FreeBSD__) && !defined __ANDROID__

#include <fhook/hook.hpp>
#include <fhook/os/unix/unix.hpp>

inline bool fhook::Hook::memoryAllocationSuccess(MemoryAllocationResult result)
{
    return (result != MAP_FAILED);
}

inline bool fhook::Hook::memoryProtectionSuccess(MemoryProtectionResult result)
{
    return (result == 0);
}

inline ErrorCode fhook::Hook::getErrorCode()
{
    return errno;
}

MemoryAllocationResult fhook::Hook::allocateMemory(size_t length, VoidPointer* address)
{
    MemoryAllocationResult mem = mmap(NULL, length, MEMORY_PROTECTION_ALL, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    *address = mem;
    return mem;
}

MemoryProtectionResult fhook::Hook::protectMemory(VoidPointer address, size_t length, MemoryProtectionFlags protection)
{
    long pageSize = sysconf(_SC_PAGESIZE);

    if(pageSize == -1) return 0;

    VoidPointer alignedAddress = (VoidPointer)( (uint64_t)address & ~(pageSize-1) );
    size_t newLength = ( (size_t)address + length ) - (size_t)alignedAddress;

    return mprotect(alignedAddress, newLength, protection);
}

#endif // OS-specific macros