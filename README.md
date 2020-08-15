# rc6

RC6 block cipher implementation from the [paper](doc/586cc5d356330aef8a868aaa6c9bee493796.pdf), fully templated to accomodate different word sizes.

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

* Fix 8/16-bit word sizes
* Pad ending blocks that do not align to word size.
* If b = 0 then c = 1 and L[0] = 0 (Look at key scheduling section of paper)
* Test different key sizes
* Add out-vector instead of doing in-place on cleartext/ciphertext
