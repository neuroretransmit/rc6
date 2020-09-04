# rc6

RC6 block cipher implementation from the [paper](doc/586cc5d356330aef8a868aaa6c9bee493796.pdf) in C++ fully templated to accomodate different word sizes. RC6 was an AES candidate finalist that was [found being used in NSA implants](https://en.wikipedia.org/wiki/RC6#Possible_use_in_NSA_%22implants%22). The algorithm was an [RSA patent](https://patents.google.com/patent/US5835600A/en) but the patent expired between 2015 and 2017.

## Features

* GCM-SIV ([RFC here](doc/rfc8452.pdf)) mode of operation
    - Authenticated Encryption with Additional Data
* Key size up to 2048 bits
* Fully parameterized to support a variety of word lengths, key sizes and number of rounds.

### Planned Features

* Flag to use [metamorphic engine from Stone Cipher-192](doc/091101.pdf)
* Parallelize

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
    // Authenticated additional data to digest for authentication
    vector<u8> aad(256);
    // Master key to be used for derivation
    vector<u8> key_generating_key(32);
    // Initialize aead for 128-bit blocks
    AEAD<BlockType::BLOCK_128> aead = AEAD<BlockType::BLOCK_128>(key_generating_key);
    
    // In place encryption, save plaintext if needed otherwise it will be modified.
    vector<u8> ciphertext = plaintext;
    
    // Encrypt
    aead.seal(plaintext, aad);
    // Decrypt and authenticate
    aead.open(ciphertext, aad);
}
```

### As a lone block cipher

```cpp
#include <rc6/mode/rc6.h>

int main()
{
    // Initialize RC6 block cipher to use 128-bit blocks
    RC6<BlockType::BLOCK_128> rc6 = RC6<BlockType::BLOCK_128>();
    
    vector<u8> block(16);
    // Create a random key, although RC5/RC6 has failed to reveal weakness in key-setup
    vector<u8> key(32);
    
    // Encrypt
    rc6.encrypt(block, key);
    // Decrypt
    rc6.decrypt(block, key);
}
```

## Building/Installation

### Requirements

* CMake >= 3.18
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
