@echo off

rem ===============        <><> Infinity Web Assembly SDK makefile (batch file for now)

rem Release build optimization level
set EXTRA_COMPILE_OPTIONS=-O2 -D__wasi__

rem ======= Configure =======

rem WASI delivers standard c/c++ runtime libraries & WASM tools.
rem Latest wasi-sdk here : https://github.com/WebAssembly/wasi-sdk/releases
rem Docs : https://wasi.dev

set LLVM_H=..\..\sys\include
set WASI_LIB=..\..\sys\wasi\lib\wasm32-wasi
set WASI_H=..\..\sys\wasi\include
set WASI_CPP_H=..\..\sys\wasi\include\c++\v1
set WASM_TOOLS=..\..\sys\bin
set CPP=%WASM_TOOLS%\clang++

rem <><> Infinity/XSG DOM interop/bootstrap headers
set INFINITY_H=..\..\sys\infinity

set COMPILE_OPTIONS=%EXTRA_COMPILE_OPTIONS%  --target=wasm32 -fno-exceptions -c 
set HEADERS=-I%WASI_CPP_H% -I%WASI_H% -I%LLVM_H% -I%INFINITY_H%

rem ======= Compile =======

if not exist obj mkdir obj

%CPP% %HEADERS% %COMPILE_OPTIONS% ./source/doodle.cpp    -o ./obj/doodle.o 

rem ======= Link =======


set LINK_OPTIONS=--no-entry --allow-undefined --export=app_init --export=app_process --export=app_terminate --export=app_move --export=app_button
set LIBS= %WASI_LIB%\libc.a  %WASI_LIB%\libc++abi.a   %WASI_LIB%\libc++.a 
%WASM_TOOLS%\wasm-ld %LINK_OPTIONS%  %LIBS% ./obj/doodle.o    -o ./obj/app.wasm


rem ======= Inspect =======

rem optional, convert to wat (text format) for manual inspection if difficulties arise.
rem wasm2wat available here : https://github.com/WebAssembly/wabt/releases
rem wasm2wat ./obj/app.wasm > ./obj/app.wat


rem ======= Install =======

rem push wasm binary into appropriate website directory for execution.
copy .\obj\app.wasm ..\..\website\visualize\doodle\_action > nul