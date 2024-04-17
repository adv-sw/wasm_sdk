#!/bin/sh

# ===============        <><> Infinity Web Assembly SDK makefile (script for now)

# Debug info please
EXTRA_COMPILE_OPTIONS=-g


# ======= Configure =======

DEST_DIR=../../website/simple/_action

# WASI delivers standard c/c++ runtime libraries
# Latest wasi-sdk here : https://github.com/WebAssembly/wasi-sdk/releases
# Docs : https://wasi.dev

WASI_LIB=../../sys/wasi/lib/wasm32-wasi
WASI_H=../../sys/wasi/include
WASI_CPP_H=../../sys/wasi/include/c++/v1

# <><> Infinty/XSG DOM interop/bootstrap headers
INFINITY_H=../../sys/infinity

COMPILE_OPTIONS="$EXTRA_COMPILE_OPTIONS --target=wasm32 -fno-exceptions -c" 
HEADERS="-I$WASI_H -I$WASI_CPP_H -I$INFINITY_H"

# ======= Compile =======

mkdir -p obj

clang $HEADERS $COMPILE_OPTIONS ./source/app.cpp   -o ./obj/app.o 


# ======= Link =======


LINK_OPTIONS="-no-entry --allow-undefined --export=app_init --export=app_process --export=app_terminate --export=app_move --export=app_button"
LIBS="$WASI_LIB/libc.a  $WASI_LIB/libc++abi.a   $WASI_LIB/libc++.a"

wasm-ld $LINK_OPTIONS  $LIBS ./obj/app.o  -o ./obj/app.wasm


# ======= Inspect =======

# optional, convert to wat (text format) for manual inspection if difficulties arise.
# wasm2wat available here : https://github.com/WebAssembly/wabt/releases
# wasm2wat ./obj/app.wasm > ./obj/app.wat
# Inspect debug info
# llvm-dwarfdump ./obj/app.wasm


# ======= Install =======

# push wasm binary into appropriate website directory for execution.
mkdir -p %DEST_DIR%
cp ./obj/app.wasm %DEST_DIR%



