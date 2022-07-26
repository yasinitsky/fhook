/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@student.uj.edu.pl>
*/

/**
 * This file contains the declaration of the main class of the library.
*/

#ifndef FHOOK_HPP
#define FHOOK_HPP

namespace fhook
{

    /**
     * Pointer to data with unspecified type
    */
    typedef void* vptr_t;

    class hook
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
            hook(vptr_t target, vptr_t trap, vptr_t nextOpcode, bool scopeDependent = false);

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
            ~hook();
        private:
            bool scopeDependent;
            bool active = false;
    };
}


#endif // FHOOK_HPP