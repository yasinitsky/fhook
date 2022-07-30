#ifndef FHOOK_TYPES_HPP
#define FHOOK_TYPES_HPP

#if defined _MSC_VER
    typedef unsigned __int8 uint8_t;
#else
    #include <stdint.h>
#endif

namespace fhook
{
    typedef void* VoidPointer;
    typedef uint8_t* BytePointer;
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


#endif // FHOOK_TYPES_HPP