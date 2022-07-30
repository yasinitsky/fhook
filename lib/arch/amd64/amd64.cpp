#if defined __amd64__ || defined _M_AMD64

#include <fhook/hook.hpp>
#include <fhook/arch/amd64/amd64.hpp>

Jump fhook::Hook::makeJump(VoidPointer address)
{
    Jump bytecode;

    bytecode.lowerBytes = ((uint32_t*) &address)[0];
    bytecode.upperBytes = ((uint32_t*) &address)[1];

    return bytecode;
}

Trampoline fhook::Hook::makeTrampoline(VoidPointer address)
{
    Trampoline bytecode;

    for(int i = 0; i < sizeof(bytecode.oldCode)/sizeof(bytecode.oldCode[0]); i++)
    {
        bytecode.oldCode[i] = 0x90; // nop
    }

    bytecode.jmp = makeJump(address);

    return bytecode;
}

#endif // CPU-specific macros