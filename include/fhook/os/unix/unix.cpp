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

template<typename PointerType>
inline bool fhook::Hook<PointerType>::memoryAllocationSuccess(MemoryAllocationResult result)
{
    return (result != MAP_FAILED);
}
template<typename PointerType>
inline bool fhook::Hook<PointerType>::memoryDeallocationSuccess(MemoryDeallocationResult result)
{
    return (result == 0);
}

template<typename PointerType>
inline bool fhook::Hook<PointerType>::memoryProtectionSuccess(MemoryProtectionResult result)
{
    return (result == 0);
}

template<typename PointerType>
inline ErrorCode fhook::Hook<PointerType>::getErrorCode()
{
    return errno;
}

template<typename PointerType>
MemoryAllocationResult fhook::Hook<PointerType>::allocateMemory(size_t length, VoidPointer* address)
{
    MemoryAllocationResult mem = mmap(NULL, length, MEMORY_PROTECTION_ALL, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    *address = mem;
    return mem;
}

template<typename PointerType>
MemoryProtectionResult fhook::Hook<PointerType>::protectMemory(VoidPointer address, size_t length, MemoryProtectionFlags protection)
{
    long pageSize = sysconf(_SC_PAGESIZE);

    if(pageSize == -1) return -1;

    VoidPointer alignedAddress = (VoidPointer)( (uint64_t)address & ~(pageSize-1) );
    size_t newLength = ( (size_t)address + length ) - (size_t)alignedAddress;

    return mprotect(alignedAddress, newLength, protection);
}

template<typename PointerType>
MemoryDeallocationResult fhook::Hook<PointerType>::deallocateMemory(VoidPointer address, size_t length)
{
    long pageSize = sysconf(_SC_PAGESIZE);

    if(pageSize == -1) return -1;

    VoidPointer alignedAddress = (VoidPointer)( (uint64_t)address & ~(pageSize-1) );
    size_t newLength = ( (size_t)address + length ) - (size_t)alignedAddress;

    return munmap(alignedAddress, newLength);
}

#endif // OS-specific macros