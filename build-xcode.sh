#!/bin/bash

rm -rf Xcode && mkdir Xcode && cd Xcode
cmake -G Xcode ..
# cmake -G Xcode -DBUILD_USE_XCODE=ON ..

echo "========================================="
echo "build xcode success"
echo "========================================="
