# Encryption-Remake

**Header‑only C++ compile‑time obfuscation using Mixed Boolean‑Arithmetic (MBA), a pseudo‑state machine with fake states, opaque predicates, and volatile variables.**

Encryption-Remake encrypts your constants and string literals at compile time.  
At runtime, a hardened decryption routine — built around a flattened state machine, MBA‑based arithmetic, and volatile opaque predicates — resists static analysis and memory dumps.  
A unique temporary buffer of randomised size is created for each obfuscated value; calling `.get()` decrypts the data, and the destructor immediately zeroes the buffer.

- **Original work:** [oxorany](https://github.com/llxiaoyuan/oxorany) by Chase (llxiaoyuan)  
- **Obfuscation techniques inspired by:** [O‑MVLL](https://obfuscator.re/omvll/)

---

## Features

- **Pure C++ header** – no external tools, just `#include`.
- **Mixed Boolean‑Arithmetic (MBA)** operations for encryption/decryption.
- **Pseudo‑state machine** with fake/bogus states – control‑flow flattening makes static tracing difficult.
- **Opaque predicates** using `volatile` variables – the compiler cannot optimise them away.
- **Random‑sized temporary buffer** – each `QTD_ENC` creates a buffer whose size varies randomly, complicating memory analysis.
- **Destructor zeroes the buffer** – sensitive data is wiped as soon as it leaves scope.
- **Single key per build** – derived from compilation time (hh:mm:ss), so every rebuild produces a different encoding while keeping the binary small.
- **Kernel‑mode ready** – define `_KERNEL_MODE` and the library uses `ntddk.h` types.
- **Debug‑mode bypass** – obfuscation is automatically disabled in `_DEBUG` builds.

---

## How it works

1. **Compile time**  
   `QTD_ENC("data")` stores encrypted bytes in a local `alignas(16)` buffer whose size is slightly randomised (padding). Every byte is encrypted with a key derived from `__TIME__` (seconds since midnight) and a round of MBA operations.

2. **Runtime**  
   `.get()` triggers a state machine (`while (true) switch(...)`) that walks through the buffer and decrypts byte by byte.  
   The machine contains:  
   - **Fake states** that perform meaningless operations but look like real decryption.  
   - **Opaque predicates** built from `volatile` variables (`X()`, `Y()`, `Z()`) – always true, but impossible for the optimiser to prove.  
   - **MBA‑based sub‑keys** generated from the base key and the index.

3. **Cleanup**  
   The destructor immediately fills the buffer with zeros, leaving no trace of the decrypted data in memory.

---

## Usage

```cpp
#include "QUOCTOANDEV.h"

int main() {
    // String obfuscation
    const char* msg = QTD_ENC("Sensitive information");
    puts(msg);

    // Integer obfuscation
    int code = QTD_ENC(0xDEADBEEF);

    // Float obfuscation (C++20 bit‑cast)
    float f   = QTD_ENC_FLT(3.14159f);

    return code;
}
