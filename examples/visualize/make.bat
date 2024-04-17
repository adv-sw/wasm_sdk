@echo off

rem ===============        <><> Infinity Web Assembly SDK makefile (batch file for now)

rem Release build optimization level
set EXTRA_COMPILE_OPTIONS=-O2 -D__wasi__

rem ======= Configure =======

set DEST_DIR="../../website/visualize/doodle/_action"

rem WASI delivers standard c/c++ runtime libraries & WASM tools.
rem Latest wasi-sdk here : https://github.com/WebAssembly/wasi-sdk/releases
rem Docs : https://wasi.dev

set WASM_SDK=../..
set COPY=call "%WASM_SDK%/bat/copy.bat"

set LLVM_H=%WASM_SDK%/sys/include
set WASI_LIB=%WASM_SDK%/sys/wasi/lib/wasm32-wasi
set WASI_H=%WASM_SDK%/sys/wasi/include
set WASI_CPP_H=%WASM_SDK%/sys/wasi/include/c++/v1
set WASM_TOOLS=%WASM_SDK%/sys/bin
set CPP="%WASM_TOOLS%/clang++"
set LINK="%WASM_TOOLS%/wasm-ld"

rem <><> Infinity/XSG DOM interop/bootstrap headers
set INFINITY_H=%WASM_SDK%/sys/infinity

set COMPILE_OPTIONS=%EXTRA_COMPILE_OPTIONS%  --target=wasm32 -fno-exceptions -c 
set HEADERS=-I%WASI_CPP_H% -I%WASI_H% -I%LLVM_H% -I%INFINITY_H%

rem ======= Compile =======

if not exist obj mkdir obj

%CPP% %HEADERS% %COMPILE_OPTIONS% ./source/doodle.cpp    -o ./obj/doodle.o 

rem ======= Link =======


set LINK_OPTIONS=--no-entry --allow-undefined --export=app_init --export=app_process --export=app_terminate --export=app_move --export=app_button
set LIBS= %WASI_LIB%/libc.a  %WASI_LIB%/libc++abi.a   %WASI_LIB%/libc++.a 
%LINK% %LINK_OPTIONS%  %LIBS% ./obj/doodle.o    -o ./obj/app.wasm


rem ======= Inspect =======

rem optional, convert to wat (text format) for manual inspection if difficulties arise.
rem wasm2wat available here : https://github.com/WebAssembly/wabt/releases
rem wasm2wat ./obj/app.wasm > ./obj/app.wat


rem ======= Install =======

rem push wasm binary into appropriate website directory for execution.
mkdir %DEST_DIR% 2> nul
%COPY% ./obj/app.wasm %DEST_DIR%