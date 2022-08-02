/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains all OS-dependent types and constants for Windows.
*/

#if defined _WIN32 || defined _WIN64
#define FHOOK_OS_INCLUDED

#ifndef FHOOK_OS_WINDOWS_HPP
#define FHOOK_OS_WINDOWS_HPP

#include <windows.h>

namespace fhook
{
    typedef LPVOID MemoryAllocationResult;
    typedef BOOL MemoryDeallocationResult;
    typedef BOOL MemoryProtectionResult;
    typedef DWORD MemoryProtectionFlags;

    const MemoryProtectionFlags MEMORY_PROTECTION_ALL = PAGE_EXECUTE_READWRITE;
    const MemoryProtectionFlags MEMORY_PROTECTION_DEFAULT = PAGE_EXECUTE_READ;

    typedef DWORD ErrorCode;
}


#endif // FHOOK_OS_WINDOWS_HPP

#endif // OS-specific macros