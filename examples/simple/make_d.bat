@echo off

rem ===============        <><> Infinity Web Assembly SDK makefile (batch file for now)

rem Debug build
set EXTRA_COMPILE_OPTIONS=-g -O0 -D__wasi__ -fno-inline


rem ======= Configure =======

rem WASI delivers standard c/c++ runtime libraries
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

%CPP% %HEADERS% %COMPILE_OPTIONS% ./source/app.cpp  -o ./obj/app.o


rem ======= Link =======



set LINK_OPTIONS=--no-entry --allow-undefined --export=app_init --export=app_process --export=app_terminate --export=app_move --export=app_msg --export=app_button
set LIBS= %WASI_LIB%\libc.a  %WASI_LIB%\libc++abi.a   %WASI_LIB%\libc++.a 

%WASM_TOOLS%\wasm-ld %LINK_OPTIONS%  %LIBS% ./obj/app.o   -o ./obj/app.wasm


rem ======= Install =======

rem Push to distribution ...
copy .\obj\app.wasm ..\..\website\simple\_action > nul

