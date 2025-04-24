cmake_minimum_required(VERSION 3.26.1)
message("--begin rtcapp complie---")
project(rtc-media)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-builtins -Wno-deprecated-declarations")

set(CMAKE_C_FLAGS_DEBUG   "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# 设置目标架构为 x86_64
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(libSRV_TRD_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty)
list(APPEND libSRV_INCLUDES_DIR ./)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/include)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/openssl/deploy/include)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/json/include)

list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libdatachannel.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libjuice.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libsrtp2.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libusrsctp.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/openssl/deploy/lib/libcrypto.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/openssl/deploy/lib/libssl.a)

file(GLOB_RECURSE CLIENT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Peer.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CLIENT_SOURCES})
add_executable(peer1-client ${CLIENT_SOURCES})
target_include_directories(peer1-client PRIVATE ${libSRV_INCLUDES_DIR})
target_link_libraries(peer1-client PRIVATE ${libs_trd_srv})


# file(GLOB_RECURSE SERVER_SOURCES 
#     ${CMAKE_CURRENT_SOURCE_DIR}/Peer2.hpp)
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SERVER_SOURCES})
# add_executable(peer2 ${SERVER_SOURCES})
# target_include_directories(peer2 PRIVATE ${libSRV_INCLUDES_DIR})
# target_link_libraries(peer2 PRIVATE ${libs_trd_srv})
