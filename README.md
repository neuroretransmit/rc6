# rc6-gcm-siv

Header only C++ implementation of the [RC6 block cipher](doc/papers/RC6_Block_Cipher_586cc5d356330aef8a868aaa6c9bee493796.pdf) implementation in [GCM-SIV operating mode](doc/papers/rfc8452.pdf)). Fully templated to accomodate different word sizes. 

## Background

RC6 was an AES candidate finalist that was [found being used in NSA implants](https://en.wikipedia.org/wiki/RC6#Possible_use_in_NSA_%22implants%22). The algorithm was an [RSA patent](https://patents.google.com/patent/US5835600A/en) but the patent expired between 2015 and 2017.

GCM-SIV was picked as a mode of operation for the following reasons.

1. Nonce misuse resistant mode, the same nonce will not produce the same message - the only information leaked is whether the two messages were equal or not.
2. Encrypted data is authenticated using additional data.
3. Parallelizable.
4. Can be implemented using AES instructions (according to paper).

## Features

* RC6
    - Key size up to 2040 bits
    - Fully parameterized to support a variety of word lengths, key sizes, and number of rounds

* GCM-SIV mode of operation
    - Authenticated Encryption with Additional Data
    - Nonce misuse resistant
    - Parallelized

## Known Issues

* `armv71` fails on 64-bit block sizes (Another little endian architecture, not sure what the deal is yet)
* Little endian untested, but the code should theoretically be in place

## Usage

### Documentation

Doxygen generated files are in [`doc/latex`](doc/latex) and [`doc/html`](doc/html).

### In GCM-SIV mode

```cpp
#include <rc6/mode/aead.h>

int main()
{
    // Secret to encrypt
    vector<u8> plaintext(256);
    // Authenticated additional data to digest
    vector<u8> aad(256);
    // Master key to be used for derivation
    vector<u8> key_generating_key(32);
    // Initialize aead for 128-bit blocks
    AEAD<BlockWordSize::BLOCK_128> aead(key_generating_key);
    
    // In place encryption, save plaintext if needed otherwise it will be modified.
    vector<u8> ciphertext = plaintext;
    
    // Encrypt
    aead.seal(ciphertext, aad);
    // aead.seal(ciphertext, aad, false); // <--- synchronous
    // Decrypt and authenticate
    aead.open(ciphertext, aad);
    // aead.open(ciphertext, aad, false); // <--- synchronous
}
```

### As a lone block cipher

```cpp
#include <rc6/cipher/rc6.h>

int main()
{
    // Initialize RC6 block cipher to use 128-bit blocks
    RC6<BlockWordSize::BLOCK_128> rc6{};
    
    vector<u8> block(block_byte_size<BlockWordSize::BLOCK_128>());
    // Create a random key
    vector<u8> key(block_byte_size<BlockWordSize::BLOCK_128>());
    
    // Encrypt
    rc6.encrypt(block, key);
    // Decrypt
    rc6.decrypt(block, key);
}
```

## Building/Installation

### Requirements

* CMake >= 3.13
* GTest
* GMock

### The Build

```bash
$ mkdir build && cd build # Create and move into build directory
$ cmake ..                # Configure
$ make                    # Build
$ sudo make install       # Install headers
```

The headers are now in `/usr/local/include/rc6`.

### Uninstall

```bash
$ sudo rm -rf /usr/local/include/rc6
```

## Running (from `build/` folder)

### Tests

Tests run a Google Test suite that test constraints of the paper as well as the test vectors.

```bash
$ ./tests/tests
[==========] Running X tests from Y test suites.
[----------] Global test environment set-up.
[----------] Z tests from RC6
[ RUN      ] ...
[       OK ] ... (0 ms)
...
...
...
[----------] Global test environment tear-down
[==========] X tests from Y test suites ran. (4 ms total)
[  PASSED  ] X tests.

...
[  PASSED  ] X tests.
```

## License

None, everyone deserves strong encryption.
