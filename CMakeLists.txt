cmake_minimum_required(VERSION 3.16.3)

set(PROJECTNAME glibc-adapter)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project(glibc-adapter C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_SYSTEM_PROCESSOR arm64)

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_ANDROID_API 30)
set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)

set(CMAKE_ANDROID_NDK "$ENV{HOME}/Android/Sdk/ndk/25.2.9519653")

set(CMAKE_ANDROID_STANDALONE_TOOLCHAIN ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/)

set(CMAKE_C_ANDROID_TOOLCHAIN_PREFIX ${CMAKE_ANDROID_STANDALONE_TOOLCHAIN}/bin/aarch64-linux-android${CMAKE_ANDROID_API}-)
set(CMAKE_C_COMPILER ${CMAKE_C_ANDROID_TOOLCHAIN_PREFIX}clang)

add_subdirectory(src)
add_subdirectory(test)

