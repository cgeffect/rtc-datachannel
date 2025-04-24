cmake_minimum_required(VERSION 3.26.1)
message("--begin rtcapp complie---")

project(rtcApp)
add_definitions(-DWEBRTC_MAC)
add_definitions(-DWEBRTC_POSIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-builtins -Wno-deprecated-declarations")

set(CMAKE_C_FLAGS_DEBUG   "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# 设置目标架构为 x86_64
set(CMAKE_OSX_ARCHITECTURES "x86_64")
# set(CMAKE_SYSTEM_PROCESSOR "x86_64;")
# set(CMAKE_OSX_ARCHITECTURES "x86_64")
# set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum OS X deployment version")
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# 查找 OpenSSL 库
find_package(OpenSSL REQUIRED)
# message("OpenSSL library status:" ${OPENSSL_FOUND} "version:" ${OPENSSL_VERSION} "include:" ${OPENSSL_INCLUDE_DIR} "lib:" ${OPENSSL_LIBRARIES})
# 检查是否找到 OpenSSL
if(OpenSSL_FOUND)
    message("Found OpenSSL:" ${OPENSSL_FOUND})
    # 输出 OpenSSL 库的路径和头文件路径，方便调试
    message("OpenSSL include dir:" ${OPENSSL_INCLUDE_DIR})
    message("OpenSSL libraries:" ${OPENSSL_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find OpenSSL")
endif()

set(libSRV_TRD_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
list(APPEND libSRV_INCLUDES_DIR ./)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/include)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/openssl/deploy/include)

list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libdatachannel.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libjuice.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libsrtp2.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/libdatachannel/deploy/lib/libusrsctp.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/openssl/deploy/lib/libcrypto.a)
list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/openssl/deploy/lib/libssl.a)

# 收集需要的头文件, 相对于CMakeLists.txt 文件所在的目录
file(GLOB_RECURSE allHEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE allCPP ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
# 在Xcode里面按照文件实际目录显示, 不要平铺
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${allCPP})

# 在 add_library 里添加 allHEADER 是为了让xcode工程显示头文件
add_executable(rtcApp ${allCPP} ${allHEADER})
target_include_directories(rtcApp PRIVATE ${libSRV_INCLUDES_DIR})
target_link_libraries(rtcApp PRIVATE OpenSSL::SSL OpenSSL::Crypto ${libs_trd_srv})

# add_executable(offer ${CMAKE_CURRENT_SOURCE_DIR}/src/offerer.cpp)
# target_include_directories(offer PRIVATE ${libSRV_INCLUDES_DIR})
# target_link_libraries(offer PRIVATE OpenSSL::SSL OpenSSL::Crypto ${libs_trd_srv})

# add_executable(answer ${CMAKE_CURRENT_SOURCE_DIR}/src/answerer.cpp)
# target_include_directories(answer PRIVATE ${libSRV_INCLUDES_DIR})
# target_link_libraries(answer PRIVATE OpenSSL::SSL OpenSSL::Crypto ${libs_trd_srv})

add_subdirectory(rtc_offer)
add_subdirectory(rtc_answer)
add_subdirectory(rtc_cs)