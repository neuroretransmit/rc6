# rc6

RC6 block cipher implementation from the [paper](doc/586cc5d356330aef8a868aaa6c9bee493796.pdf), fully templated to accomodate different word sizes. RC6 is an AES (Advanced Encryption Standard) candidate that was found being used in NSA implants.

## Requirements

To run tests you need GTest and GMock installed on your system.

## Building

```
$ mkdir build && cd build
$ cmake ..
$ make
```

## Running (from `build/` folder)

* Main program
    - `./src/rc6`
* Tests
    - `./tests/tests`

## TODO

* Read in bytes instead of word size vector for encryption/decryption
* Mode of operation (accomodate 256 bit blocks parallelized)
* Verify that if b = 0 then c = 1 and L[0] = 0 (Look at key scheduling section of paper)
* Test different key sizes
* Add out-vector instead of doing in-place on cleartext/ciphertext
* Flag to use metamorphic engine from Stone cipher
