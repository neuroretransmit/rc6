# rc6

RC6 block cipher implementation from the [paper](doc/586cc5d356330aef8a868aaa6c9bee493796.pdf), fully templated to accomodate different word sizes. RC6 is an AES (Advanced Encryption Standard) candidate that was found being used in NSA implants.

## Usage

Copy [`types.h`](src/types.h), [`binops.h`](src/binops.h), and [`rc6.h`](src/rc6.h) to your source directory.

```cpp
#include "path/to/rc6.h"

using namespace rc6;

void int main()
{
    // Initialize RC6 block cipher to use 128-bit blocks (u32 * 4)
    RC6<u32> rc6 = RC6<u32>();
    // Encrypt/Decrypt take in a vector of bytes, u8 is defined in types.h
    vector<u8> block(16);
    // Please create a random key, although RC6 has no known key weaknesses
    vector<u8> key(16);
    
    rc6.encrypt(block, key);
    rc6.decrypt(block, key);
}
```

## Building

### Requirements

* CMake >= 3.18
* GTest
* GMock

### The Build

```
$ mkdir build && cd build
$ cmake ..
$ make
```

## Running (from `build/` folder)

### Main program

The `main.cc` file runs the test vectors from the whitepaper.

```bash
$./src/rc6
========== TEST VECTOR #1 ==========
PLAIN   : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
KEY     : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ENCRYPT : 8f c3 a5 36 56 b1 f7 78 c1 29 df 4e 98 48 a4 1e
DECRYPT : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
========== TEST VECTOR #2 ==========
PLAIN   : 02 13 24 35 46 57 68 79 8a 9b ac bd ce df e0 f1
KEY     : 01 23 45 67 89 ab cd ef 01 12 23 34 45 56 67 78
ENCRYPT : 52 4e 19 2f 47 15 c6 23 1f 51 f6 36 7e a4 3f 18
DECRYPT : 02 13 24 35 46 57 68 79 8a 9b ac bd ce df e0 f1
========== TEST VECTOR #3 ==========
PLAIN   : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
KEY     : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
                      00 00 00 00 00 00 00 00
ENCRYPT : 6c d6 1b cb 19 0b 30 38 4e 8a 3f 16 86 90 ae 82
DECRYPT : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
========== TEST VECTOR #4 ==========
PLAIN   : 02 13 24 35 46 57 68 79 8a 9b ac bd ce df e0 f1
KEY     : 01 23 45 67 89 ab cd ef 01 12 23 34 45 56 67 78
                      89 9a ab bc cd de ef f0
ENCRYPT : 68 83 29 d0 19 e5 05 04 1e 52 e9 2a f9 52 91 d4
DECRYPT : 02 13 24 35 46 57 68 79 8a 9b ac bd ce df e0 f1
========== TEST VECTOR #5 ==========
PLAIN   : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
KEY     : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
          00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
ENCRYPT : 8f 5f bd 05 10 d1 5f a8 93 fa 3f da 6e 85 7e c2
DECRYPT : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
========== TEST VECTOR #6 ==========
PLAIN   : 02 13 24 35 46 57 68 79 8a 9b ac bd ce df e0 f1
KEY     : 01 23 45 67 89 ab cd ef 01 12 23 34 45 56 67 78
          89 9a ab bc cd de ef f0 10 32 54 76 98 ba dc fe
ENCRYPT : c8 24 18 16 f0 d7 e4 89 20 ad 16 a1 67 4e 5d 48
DECRYPT : 02 13 24 35 46 57 68 79 8a 9b ac bd ce df e0 f1
```
### Tests

Tests run a Google Test suite that test constraints of the paper as well as the test vectors.

```bash
$ ./tests/tests
[==========] Running 13 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 11 tests from RC6
[ RUN      ] RC6.MagicConstantP32
[       OK ] RC6.MagicConstantP32 (0 ms)
[ RUN      ] RC6.MagicConstantQ32
[       OK ] RC6.MagicConstantQ32 (0 ms)
[ RUN      ] RC6.KeyAbove2048Bits
[       OK ] RC6.KeyAbove2048Bits (2 ms)
[ RUN      ] RC6.WordSize32Bit
[       OK ] RC6.WordSize32Bit (0 ms)
[ RUN      ] RC6.WordSize64Bit
[       OK ] RC6.WordSize64Bit (0 ms)
[ RUN      ] RC6.PaperTestVector1
[       OK ] RC6.PaperTestVector1 (0 ms)
[ RUN      ] RC6.PaperTestVector2
[       OK ] RC6.PaperTestVector2 (0 ms)
[ RUN      ] RC6.PaperTestVector3
[       OK ] RC6.PaperTestVector3 (0 ms)
[ RUN      ] RC6.PaperTestVector4
[       OK ] RC6.PaperTestVector4 (0 ms)
[ RUN      ] RC6.PaperTestVector5
[       OK ] RC6.PaperTestVector5 (0 ms)
[ RUN      ] RC6.PaperTestVector6
[       OK ] RC6.PaperTestVector6 (0 ms)
[----------] 11 tests from RC6 (2 ms total)

[----------] 2 tests from BinOps
[ RUN      ] BinOps.ROL
[       OK ] BinOps.ROL (0 ms)
[ RUN      ] BinOps.ROR
[       OK ] BinOps.ROR (0 ms)
[----------] 2 tests from BinOps (0 ms total)

[----------] Global test environment tear-down
[==========] 13 tests from 2 test suites ran. (2 ms total)
[  PASSED  ] 13 tests.
```

## TODO

* Implement GCM-SIV as mode of operation
* Flag to use metamorphic engine from Stone cipher
