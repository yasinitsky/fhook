/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains the declaration of the main class of the library.
*/

#ifndef FHOOK_HOOK_HPP
#define FHOOK_HOOK_HPP

#include "types.hpp"

#include <iostream>
#include <cstdio>

namespace fhook
{

    template<typename PointerType>
    class Hook
    {
        public:
            /**
             * Create a hook with specific parameters.
             * 
             * target - pointer to the function to be hooked
             * 
             * trap - pointer to the function to be called instead of target, must have the same calling convention as the original function
             * 
             * nextOpcode - pointer to the first opcode of the target function, which is located after the address (target + sizeof(fhook::Jump))
             * 
             * scopeDependent - remove the hook in the destructor?
             * 
             * throw: NotEnoughMemoryException
            */
            Hook(PointerType target, PointerType trap, size_t nextOpcodeOffset, bool scopeDependent = false);

            /**
             * Install a hook.
             * 
             * throw: MemoryAllocateException, MemoryProtectException
            */

            PointerType install();

            /**
             * Remove a hook.
             * 
             * throw: MemoryProtectException, MemoryDeallocateException
            */
            void remove();

            /**
             * Hook object destructor.
             * 
             * If scopeDependent is true, remove the hook from the function. Otherwise, leave the hook.
            */
            ~Hook();
        private:
            bool scopeDependent;
            bool active = false;
            VoidPointer target;
            VoidPointer trap;
            VoidPointer nextOpcode;
            Trampoline* trampolineAddr = nullptr;

            void memoryCopy(VoidPointer destination, VoidPointer source, size_t length);
            LongestInteger getDisplacement(VoidPointer first, VoidPointer second);
            void memoryDump(VoidPointer address, size_t length);

            inline bool memoryAllocationSuccess(MemoryAllocationResult result);
            inline bool memoryDeallocationSuccess(MemoryDeallocationResult result);
            inline bool memoryProtectionSuccess(MemoryProtectionResult result);

            inline ErrorCode getErrorCode();

            MemoryAllocationResult allocateMemory(size_t length, VoidPointer* address);
            MemoryDeallocationResult deallocateMemory(VoidPointer address, size_t length);
            MemoryProtectionResult protectMemory(VoidPointer address, size_t length, MemoryProtectionFlags protection);
            
            Jump makeJump(VoidPointer address);
            Trampoline makeTrampoline(VoidPointer address);
    };
}

#include "fhook.tpp"
#include "os/unix/unix.tpp"
#include "arch/amd64/amd64.tpp"

#endif // FHOOK_HOOK_HPP