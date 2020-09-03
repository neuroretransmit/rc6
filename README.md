# rc6

RC6 block cipher implementation from the [paper](doc/586cc5d356330aef8a868aaa6c9bee493796.pdf) in C++ fully templated to accomodate different word sizes. RC6 was an AES candidate finalist that was [found being used in NSA implants](https://en.wikipedia.org/wiki/RC6#Possible_use_in_NSA_%22implants%22). The algorithm was an [RSA patent](https://patents.google.com/patent/US5835600A/en) but the patent expired between 2015 and 2017.

## Features

* GCM-SIV ([RFC here](doc/rfc8452.pdf)) mode of operation
    - Authentication AND encryption
* Key size up to 2048 bits
* Fully parameterized to support a variety of word lengths, key sizes and number of rounds.

### Planned Features

* Flag to use [metamorphic engine from Stone Cipher-192](doc/091101.pdf)
* Parallelize

## Usage

Doxygen generated files are in `doc/latex` and `doc/html`.

Copy [`types.h`](src/types.h), [`binops.h`](src/binops.h), and [`rc6.h`](src/rc6.h) to your source directory.

### In GCM-SIV mode

```cpp
#include "rc6/mode/aead.h"

int main()
{
    vector<u8> plaintext(8 * 32);
    vector<u8> aad(256);
    vector<u8> key_generating_key(16);
    AEAD<BlockType::BLOCK_128> aead = AEAD<BlockType::BLOCK_128>(key_generating_key);
    vector<u8> ciphertext = plaintext;
    
    // Encrypt
    aead.seal(ciphertext, aad);
    // Decrypt and authenticate
    aead.open(ciphertext, aad);
```

### As a lone block cipher

```cpp
#include "rc6/mode/rc6.h"

int main()
{
    // Initialize RC6 block cipher to use 128-bit blocks
    RC6<BlockType::BLOCK_128> rc6 = RC6<BlockType::BLOCK_128>();
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
$ ./src/rc6_test_vectors
########### RC6 BLOCK CIPHER ###########
========== TEST VECTOR #1 ==========
PLAIN	: 00000000000000000000000000000000
KEY	: 00000000000000000000000000000000
ENCRYPT	: 8fc3a53656b1f778c129df4e9848a41e
DECRYPT	: 00000000000000000000000000000000
========== TEST VECTOR #2 ==========
PLAIN	: 02132435465768798a9bacbdcedfe0f1
KEY	: 0123456789abcdef0112233445566778
ENCRYPT	: 524e192f4715c6231f51f6367ea43f18
DECRYPT	: 02132435465768798a9bacbdcedfe0f1
========== TEST VECTOR #3 ==========
PLAIN	: 00000000000000000000000000000000
KEY	: 000000000000000000000000000000000000000000000000
ENCRYPT	: 6cd61bcb190b30384e8a3f168690ae82
DECRYPT	: 00000000000000000000000000000000
========== TEST VECTOR #4 ==========
PLAIN	: 02132435465768798a9bacbdcedfe0f1
KEY	: 0123456789abcdef0112233445566778899aabbccddeeff0
ENCRYPT	: 688329d019e505041e52e92af95291d4
DECRYPT	: 02132435465768798a9bacbdcedfe0f1
========== TEST VECTOR #5 ==========
PLAIN	: 00000000000000000000000000000000
KEY	: 0000000000000000000000000000000000000000000000000000000000000000
ENCRYPT	: 8f5fbd0510d15fa893fa3fda6e857ec2
DECRYPT	: 00000000000000000000000000000000
========== TEST VECTOR #6 ==========
PLAIN	: 02132435465768798a9bacbdcedfe0f1
KEY	: 0123456789abcdef0112233445566778899aabbccddeeff01032547698badcfe
ENCRYPT	: c8241816f0d7e48920ad16a1674e5d48
DECRYPT	: 02132435465768798a9bacbdcedfe0f1

########### AEAD MODE OF OPERATION ###########
PLAIN	: 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
KEY	: 00000000000000000000000000000000
ENCRYPT	: a6f0d82981c7d7fd424c9754ed6d6c2c24929b1b90893c238c0c638cb24cef27d943847fba577de2ec508d20e0f44346e6d10a26174b55d79421d2ea430ffa71b8a33cdb783ed79f1d0fc93f78dd7d0f2b3779da68353cbbed8845d1586834c8a77e9b70acb87b94ee54c30064b2545b9a5bbb74fa80f31ab6e1e533a8baccd575a8113d14c03f9bcbd5d48b4137aae4a55e755d0246cd20a8f8bdf57428c9f98623d934aad9817efe06b2dbbe5b3c875db886a5464a90fd17bf4790eacb7dde6917bc1f1c53587cb56e05dd0fc38906781c765a7f4ac780d29942671f75b2d9a17c31fb1f5bfbda0508b4e4e5ca662be52bbccaa260fed8817f86fd780a77f63c6f19a7e778dbe60188916837ee78ac971addb63efe892e74e9ff0d
DECRYPT	: 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
```
### Tests

Tests run a Google Test suite that test constraints of the paper as well as the test vectors.

```bash
$ ./tests/tests
[==========] Running 39 tests from 6 test suites.
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
...
[  PASSED  ] 39 tests.
```

## License

None, everyone deserves strong encryption.
