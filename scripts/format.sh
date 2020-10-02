#!/bin/sh

clang-format -i $(find include/ -name '**.h') $(find tests/ -name '**.cc')

