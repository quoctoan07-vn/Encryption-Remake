# Encryption-Remake
C++ compile-time obfuscation library using Mixed Boolean-Arithmetic (MBA), a pseudo-state machine with fake states, opaque predicates, and volatile variables to resist static analysis. The QTD_ENC macro creates a temporary encrypted buffer of random size; calling get() decrypts the data, and the destructor zeroes the buffer to thwart memory dumps.
