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

#include "types.hpp"

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
             * nextOpcode - pointer to the first opcode of the target function, which is located after the address (target + FHOOK_TRAMPOLINE_SIZE),
             * where FHOOK_TRAMPOLINE_SIZE - CPU-specific jump operation size
             * 
             * scopeDependent - remove the hook in the destructor?
            */
            Hook(VoidPointer target, VoidPointer trap, VoidPointer nextOpcode, bool scopeDependent = false) : target(target), trap(trap), nextOpcode(nextOpcode), scopeDependent(scopeDependent) {}

            /**
             * Install a hook.
            */
            void install();

            /**
             * Remove a hook.
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
            VoidPointer target, trap, nextOpcode;

            MemoryAllocationResult allocateMemory(std::size_t bytes);
            MemoryProtectionResult protectMemoty(VoidPointer address);
            
    };
}


#endif // FHOOK_GENERAL_HPP