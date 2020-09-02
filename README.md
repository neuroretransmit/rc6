# rc6

RC6 block cipher implementation from the [paper](doc/586cc5d356330aef8a868aaa6c9bee493796.pdf) in C++ fully templated to accomodate different word sizes. RC6 was an AES candidate finalist that was [found being used in NSA implants](https://en.wikipedia.org/wiki/RC6#Possible_use_in_NSA_%22implants%22). The algorithm was an [RSA patent](https://patents.google.com/patent/US5835600A/en) but the patent expired between 2015 and 2017.

## Features

* Key size up to 2040 bits
* Fully parameterized to support a variety of word-lengths, key sizes and number of rounds.

### Planned Features

* In progress: [GCM-SIV](doc/rfc8452.pdf) as mode of operation. (Implemented POLYVAL/AEAD, implementing ECB/CTR modes to fully execute GCM-SIV)
* Flag to use [metamorphic engine from Stone Cipher-192](doc/091101.pdf)

## Usage

Doxygen generated files are in `doc/latex` and `doc/html`.

Copy [`types.h`](src/types.h), [`binops.h`](src/binops.h), and [`rc6.h`](src/rc6.h) to your source directory.

```cpp
#include "path/to/rc6.h"

using namespace rc6;

int main()
{
    // Initialize RC6 block cipher to use 128-bit blocks (u32 * 4)
    RC6<u32> rc6 = RC6<u32>();
    // Encrypt/Decrypt take in a vector of bytes, u8 is defined in types.h
    vector<u8> block(16);
    // Please create a random key, although RC5/RC6 has failed to reveal weakness in key-setup
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
[==========] Running 37 tests from 5 test suites.
[----------] Global test environment set-up.
[----------] 11 tests from RC6
[ RUN      ] RC6.MagicConstantP32
[       OK ] RC6.MagicConstantP32 (0 ms)
[ RUN      ] RC6.MagicConstantQ32
[       OK ] RC6.MagicConstantQ32 (0 ms)
[ RUN      ] RC6.KeyAbove2048Bits
[       OK ] RC6.KeyAbove2048Bits (1 ms)
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
[----------] 11 tests from RC6 (1 ms total)

[----------] 3 tests from BinOps
[ RUN      ] BinOps.ROL
[       OK ] BinOps.ROL (0 ms)
[ RUN      ] BinOps.ROR
[       OK ] BinOps.ROR (0 ms)
[ RUN      ] BinOps.IsBigEndian
[       OK ] BinOps.IsBigEndian (0 ms)
[----------] 3 tests from BinOps (0 ms total)

[----------] 2 tests from AEAD
[ RUN      ] AEAD.DeriveKeys128
[       OK ] AEAD.DeriveKeys128 (0 ms)
[ RUN      ] AEAD.DeriveKeys256
[       OK ] AEAD.DeriveKeys256 (0 ms)
[----------] 2 tests from AEAD (0 ms total)

[----------] 8 tests from FieldElement64
[ RUN      ] FieldElement64.ConstructorFromString
[       OK ] FieldElement64.ConstructorFromString (0 ms)
[ RUN      ] FieldElement64.ConstructorFromBytes
[       OK ] FieldElement64.ConstructorFromBytes (0 ms)
[ RUN      ] FieldElement64.ConstructorFromBytesWithOffset
[       OK ] FieldElement64.ConstructorFromBytesWithOffset (0 ms)
[ RUN      ] FieldElement64.ConstructorFromLongs
[       OK ] FieldElement64.ConstructorFromLongs (0 ms)
[ RUN      ] FieldElement64.str
[       OK ] FieldElement64.str (0 ms)
[ RUN      ] FieldElement64.bytes
[       OK ] FieldElement64.bytes (0 ms)
[ RUN      ] FieldElement64.OperatorAdd
[       OK ] FieldElement64.OperatorAdd (0 ms)
[ RUN      ] FieldElement64.OperatorMul
[       OK ] FieldElement64.OperatorMul (1 ms)
[----------] 8 tests from FieldElement64 (1 ms total)

[----------] 13 tests from Polyval
[ RUN      ] Polyval.ConstructorFromHLongs
[       OK ] Polyval.ConstructorFromHLongs (0 ms)
[ RUN      ] Polyval.ConstructorFromHAndSLongs
[       OK ] Polyval.ConstructorFromHAndSLongs (0 ms)
[ RUN      ] Polyval.ConstructFromOwnDigest
[       OK ] Polyval.ConstructFromOwnDigest (0 ms)
[ RUN      ] Polyval.ConstructorFromHBytes
[       OK ] Polyval.ConstructorFromHBytes (0 ms)
[ RUN      ] Polyval.ConstructorFromHAndSBytes
[       OK ] Polyval.ConstructorFromHAndSBytes (0 ms)
[ RUN      ] Polyval.ConstructorFromHString
[       OK ] Polyval.ConstructorFromHString (0 ms)
[ RUN      ] Polyval.ConstructorFromHAndSStrings
[       OK ] Polyval.ConstructorFromHAndSStrings (0 ms)
[ RUN      ] Polyval.UpdateBlock0
[       OK ] Polyval.UpdateBlock0 (0 ms)
[ RUN      ] Polyval.UpdateBlock
[       OK ] Polyval.UpdateBlock (0 ms)
[ RUN      ] Polyval.UpdateNotDependingOnZeroBlock
[       OK ] Polyval.UpdateNotDependingOnZeroBlock (0 ms)
[ RUN      ] Polyval.Update
[       OK ] Polyval.Update (0 ms)
[ RUN      ] Polyval.PadWithZeroes
[       OK ] Polyval.PadWithZeroes (0 ms)
[ RUN      ] Polyval.ResetSToZero
[       OK ] Polyval.ResetSToZero (0 ms)
[----------] 13 tests from Polyval (0 ms total)

[----------] Global test environment tear-down
[==========] 37 tests from 5 test suites ran. (2 ms total)
[  PASSED  ] 37 tests.
```

## License

None, everyone deserves strong encryption.
