#!/bin/bash

echo "========================================="
echo "build xcode"
echo "========================================="
cd libdatachannel && \
rm -rf Xcode && mkdir Xcode && cd Xcode
cmake -G Xcode .. -DUSE_GNUTLS=0 -DUSE_NICE=0
# cmake -G Xcode -DBUILD_USE_XCODE=ON ..

echo "========================================="
echo "build xcode success"
echo "========================================="
