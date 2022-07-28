/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file includes all header files in the correct order. The main header file of the library.
*/

#ifndef FHOOK_HPP
#define FHOOK_HPP

namespace fhook
{
    typedef void* VoidPointer;
}

/**
 * OS-specific headers.
 * To add support for a new operating system, just add its header file here
 * (provided that the OS header file is written according to the documentation).
*/
#include "os/unix/unix.hpp"

#if !defined FHOOK_OS_INCLUDED
    #error "fhook: unsupported operating system"
#endif


/**
 * CPU-specific headers.
 * To add support for a new architecture, just add its header file here
 * (provided that the architecture header file is written according to the documentation).
*/
#include "arch/amd64/amd64.hpp"

#if !defined FHOOK_ARCH_INCLUDED
    #error "fhook: unsupported architecture"
#endif

#include "general.hpp"

#endif // FHOOK_HPP