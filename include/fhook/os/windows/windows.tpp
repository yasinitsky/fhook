/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains the implementation of OS-dependent methods for Windows.
*/

#if defined(_WIN32)

#include <fhook/hook.hpp>
#include <fhook/os/windows/windows.hpp>

#include <memoryapi.h>
#include <windows.h>
#include <sysinfoapi.h>

template<typename PointerType>
inline bool fhook::Hook<PointerType>::memoryAllocationSuccess(MemoryAllocationResult result)
{
    return (result != NULL);
}
template<typename PointerType>
inline bool fhook::Hook<PointerType>::memoryDeallocationSuccess(MemoryDeallocationResult result)
{
    return (result != 0);
}

template<typename PointerType>
inline bool fhook::Hook<PointerType>::memoryProtectionSuccess(MemoryProtectionResult result)
{
    return (result != 0);
}

template<typename PointerType>
inline ErrorCode fhook::Hook<PointerType>::getErrorCode()
{
    return GetLastError();
}

template<typename PointerType>
MemoryAllocationResult fhook::Hook<PointerType>::allocateMemory(size_t length, VoidPointer* address)
{
    LPVOID mem = VirtualAlloc(NULL, (SIZE_T)length, MEM_COMMIT | MEM_RESERVE, MEMORY_PROTECTION_ALL);
    *address = (VoidPointer) mem;
    return mem;
}

template<typename PointerType>
MemoryProtectionResult fhook::Hook<PointerType>::protectMemory(VoidPointer address, size_t length, MemoryProtectionFlags protection)
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    DWORD pageSize = sysinfo.dwPageSize;
    DWORD stub;

    VoidPointer alignedAddress = (VoidPointer)( (DWORD64)address & ~((DWORD64)pageSize - 1) );

    size_t newLength = ( (size_t)address + length ) - (size_t)alignedAddress;

    return VirtualProtect((LPVOID)alignedAddress, (SIZE_T)newLength, protection, &stub);
}

template<typename PointerType>
MemoryDeallocationResult fhook::Hook<PointerType>::deallocateMemory(VoidPointer address, size_t length)
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    DWORD pageSize = sysinfo.dwPageSize;

    VoidPointer alignedAddress = (VoidPointer)( (DWORD64)address & ~( (DWORD64)pageSize - 1 ) );
    size_t newLength = ( (size_t)address + length ) - (size_t)alignedAddress;

    return VirtualFree((LPVOID)alignedAddress, 0, MEM_RELEASE);
}

#endif // OS-specific macros