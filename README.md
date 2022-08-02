# fhook - C++ hook library

A function hooking library with a clear separation of OS-dependent and architecture-dependent parts of the code, which makes it easily portable.

# Advantages
 - Simple and convenient API interface.
 - Easy portability to other platforms.
 - Include-only.
 - BSD Zero Clause License.

# Currently supported platforms
 - Linux x86-64.
 - FreeBSD, NetBSD, OpenBSD x86-64.
 - Windows x86-64.

# Installation

 1. Copy `fhook` directory to your includes folder.
 2. Include `fhook/fhook.hpp` in source file.

# Usage
Imagine that we have a function `int foo(int a, int b)`, the address of which we know and we want to hook it.

First, let's disassemble our function:

    0000000000001329 <foo>:
    1329:       f3 0f 1e fa             endbr64
    132d:       55                      push   rbp
    132e:       48 89 e5                mov    rbp,rsp
    1331:       48 83 ec 10             sub    rsp,0x10
    1335:       89 7d fc                mov    DWORD PTR [rbp-0x4],edi
    1338:       89 75 f8                mov    DWORD PTR [rbp-0x8],esi
    // ...

The size of the `fhook::Jump` structure in the x86-64 architecture is 14 bytes (we can see this in the file `fhook/arch/amd64/amd64.hpp`). Accordingly, we need to look for the beginning of the instruction at offset 15 from the beginning of the function. In our case, at the address `foo+15` is the beginning of a new instruction (`mov DWORD PTR [rbp-0x4],edi`). Otherwise, we need to look for the beginning of the instruction at address `foo+16`, `foo+17`, etc., until we find. In total, the offset of the first "useful" opcode is 15. Let's code!
## C++17
```cpp
#include <iostream>
#include <fhook/fhook.hpp>

int (*fooAddr)(int, int); // address of original foo

int hookedFoo(int a, int b)
{
	std::cout << "a" << " " << b << std::endl;
	return fooAddr(a, b);	// calling the original function
}

int main()
{
	fhook::Hook fooHook(fooAddr, hookedFoo, 15); // 15 - offset obtained by disassembly
	
	fooAddr = fooHook.install(); // installing the hook
	// now all calls to the original function will be redirected to the interceptor function
	// we can still call the original function using the fooAddr pointer
	
	fooHook.remove(); // removing the hook
	// note that the address in the fooAddr is no longer valid
}
```
## C++11
If for some reason you want to use C++11, please note that you need to explicitly specify the type of the function in the class `fhook::Hook` template arguments:
```cpp
// changes for the example above
fhook::Hook<int (*)(int, int)> fooHook(fooAddr, hookedFoo, 15);
```
# License
BSD Zero Clause Licence.

This means that you can use, modify and distribute this product in any way you like without the requirement to credit the author.
