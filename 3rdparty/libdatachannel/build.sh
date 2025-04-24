#!/bin/sh
workdir=$(cd $(dirname $0); pwd)
rootdir=$(cd $workdir/../../; pwd)

src_dir_inside=libdatachannel

rm -rf "$workdir/deploy" \
rm -rf "$workdir/${src_dir_inside}" && mkdir -p "$workdir/${src_dir_inside}" && cd "$workdir" && \
tar -zxvf "$workdir/source/${src_dir_inside}.tar.gz"

echo "--- build ${package} ---"
############ build ${package} ############
mkdir -p "$workdir/${src_dir_inside}/build" && cd "$workdir/${src_dir_inside}/build" && \
cmake \
    -DBUILD_SHARED_LIBS=OFF \
    -DUSE_GNUTLS=OFF \
    -DUSE_NICE=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="x86_64" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="11.0" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_INSTALL_PREFIX="$workdir/deploy" .. && \
make -j8 && make install

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build ${package}"
    exit -1
fi

echo "success!"
