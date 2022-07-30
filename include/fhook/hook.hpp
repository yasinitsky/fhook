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

#ifndef FHOOK_GENERAL_HPP
#define FHOOK_GENERAL_HPP

#include <iostream>
#include <cstdio>

namespace fhook
{

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
            */
            Hook(VoidPointer target, VoidPointer trap, VoidPointer nextOpcode, bool scopeDependent = false) : target(target), trap(trap), nextOpcode(nextOpcode), scopeDependent(scopeDependent)
            {
                if((getDisplacement(nextOpcode, target)) < sizeof(Jump))
                    throw NotEnoughMemoryException();
            }

            VoidPointer install()
            {
                Trampoline* trampolineAddr;

                MemoryAllocationResult alloc = allocateMemory(sizeof(Trampoline), (VoidPointer*) &trampolineAddr);
                MemoryProtectionResult protect;
                Trampoline trampoline;
                Jump jump;

                if(!memoryAllocationSuccess(alloc))
                    throw MemoryAllocateException(getErrorCode());

                this->trampolineAddr = trampolineAddr;

                protect = protectMemory(trampolineAddr, sizeof(Trampoline), MEMORY_PROTECTION_ALL);

                if(!memoryProtectionSuccess(protect))
                    throw MemoryProtectException(getErrorCode());

                protect = protectMemory(target, sizeof(Jump), MEMORY_PROTECTION_ALL);

                if(!memoryProtectionSuccess(protect))
                    throw MemoryProtectException(getErrorCode());

                trampoline = makeTrampoline(nextOpcode);
                jump = makeJump(trap);

                memoryCopy((VoidPointer) trampoline.oldCode, target, getDisplacement(nextOpcode, target));
                memoryCopy((VoidPointer) trampolineAddr, (VoidPointer) &trampoline, sizeof(Trampoline));

                memoryCopy(target, (VoidPointer) &jump, sizeof(Jump));

                memoryDump(trampolineAddr, sizeof(Trampoline));

                return trampolineAddr;
            }

            /**
             * Remove a hook.
            */
            void remove();

            /**
             * Hook object destructor.
             * 
             * If scopeDependent is true, remove the hook from the function. Otherwise, leave the hook.
            */
            ~Hook() {}
        private:
            bool scopeDependent;
            bool active = false;
            VoidPointer target;
            VoidPointer trap;
            VoidPointer nextOpcode;
            Trampoline* trampolineAddr = nullptr;

            void memoryCopy(VoidPointer destination, VoidPointer source, size_t length)
            {
                BytePointer destinationByte = (BytePointer) destination;
                BytePointer sourceByte = (BytePointer) source;

                for(int i = 0; i < length; i++)
                {
                    destinationByte[i] = sourceByte[i];
                }
            }

            LongestInteger getDisplacement(VoidPointer first, VoidPointer second)
            {
                return (LongestInteger) ( (BytePointer)first - (BytePointer)second );
            }

        public:
            void memoryDump(VoidPointer address, size_t length)
            {
                for(size_t i = 0; i < length; i++)
                {
                    printf("%02hhx ", ((BytePointer) address)[i]);
                }

                printf("\n");
            }
            
    };
}


#endif // FHOOK_GENERAL_HPP