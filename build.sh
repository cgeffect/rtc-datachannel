#!/bin/bash
workdir=$(cd $(dirname $0); pwd -P)

rm -rf build && mkdir -p build && cd build && \
cmake .. && make -j8

echo "========================================="
echo "build all success"
echo "========================================="
