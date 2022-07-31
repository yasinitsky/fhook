/**
 * This file is part of the fhook library.
 * 
 * SPDX-License-Identifier: 0BSD
 * 
 * Copyright (C) 2022
 * Author: Yaroslav Yasynytskyi <yaroslav.yasynytskyi@gmail.com>
*/

/**
 * This file contains the implementation of all platform-independent methods of the hook class.
*/

#include <fhook/hook.hpp>
#include <fhook/exceptions.hpp>

using namespace fhook;

template<typename PointerType>
Hook<PointerType>::Hook(PointerType target, PointerType trap, size_t nextOpcodeOffset, bool scopeDependent) : scopeDependent(scopeDependent)
{
    this->target = (VoidPointer)target;
    this->trap = (VoidPointer)trap;
    nextOpcode = (VoidPointer)( (size_t)this->target + nextOpcodeOffset );

    if(getDisplacement(nextOpcode, this->target) < sizeof(Jump))
        throw NotEnoughMemoryException();
}

template<typename PointerType>
PointerType Hook<PointerType>::install()
{
    if(active) return (PointerType)trampolineAddr;

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

    std::cout << nextOpcode << " " << target << std::endl;

    memoryCopy((VoidPointer) trampoline.oldCode, target, getDisplacement(nextOpcode, target));
    memoryCopy((VoidPointer) trampolineAddr, (VoidPointer) &trampoline, sizeof(Trampoline));
    memoryCopy(target, (VoidPointer) &jump, sizeof(Jump));

    active = true;

    return (PointerType)trampolineAddr;
}

template<typename PointerType>
void Hook<PointerType>::remove()
{
    if(!active) return;

    MemoryProtectionResult protect;
    MemoryDeallocationResult dealloc;
    size_t bytesToRestore = getDisplacement(nextOpcode, target);
    memoryCopy(target, trampolineAddr->oldCode, bytesToRestore);

    active = false;

    protect = protectMemory(target, bytesToRestore, MEMORY_PROTECTION_DEFAULT);

    if(!memoryProtectionSuccess(protect))
        throw MemoryProtectException(getErrorCode());

    dealloc = deallocateMemory(trampolineAddr, sizeof(Trampoline));

    if(!memoryDeallocationSuccess(dealloc))
        throw MemoryProtectException(getErrorCode());
    

}

template<typename PointerType>
void Hook<PointerType>::memoryCopy(VoidPointer destination, VoidPointer source, size_t length)
{
    BytePointer destinationByte = (BytePointer) destination;
    BytePointer sourceByte = (BytePointer) source;

    for(int i = 0; i < length; i++)
    {
        destinationByte[i] = sourceByte[i];
    }
}

template<typename PointerType>
LongestInteger Hook<PointerType>::getDisplacement(VoidPointer first, VoidPointer second)
{
    return ( (LongestInteger)first - (LongestInteger)second );
}

template<typename PointerType>
void Hook<PointerType>::memoryDump(VoidPointer address, size_t length)
{
    for(size_t i = 0; i < length; i++)
    {
        printf("%02hhx ", ((BytePointer) address)[i]);
    }

    printf("\n");
}

template<typename PointerType>
Hook<PointerType>::~Hook()
{
    if(scopeDependent && active) remove();
}

#include "os/unix/unix.cpp"
#include "arch/amd64/amd64.cpp"