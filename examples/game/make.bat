rem ===============        <><> Infinity Web Assembly SDK makefile (batch file for now)

rem Release build optimization level
set EXTRA_COMPILE_OPTIONS=-O0

rem ======= Configure =======

rem WASI delivers standard c/c++ runtime libraries
rem Latest wasi-sdk here : https://github.com/WebAssembly/wasi-sdk/releases
rem Docs : https://wasi.dev

set WASI_LIB=..\..\sys\wasi\lib\wasm32-wasi
set WASI_H=..\..\sys\wasi\include
set WASI_CPP_H=..\..\sys\wasi\include\c++\v1
set WASI_SDK=..\..\sys\wasi\sdk\bin

rem <><> Infinity/XSG DOM interop/bootstrap headers
set INFINITY_H=..\..\sys\infinity

set COMPILE_OPTIONS=%EXTRA_COMPILE_OPTIONS% --target=wasm32 -fno-exceptions -c 
set HEADERS=-I%WASI_H% -I%WASI_CPP_H% -I%INFINITY_H%

rem ======= Compile =======

if not exist obj mkdir obj

%WASI_SDK%\clang %HEADERS% %COMPILE_OPTIONS% ./source/inf_entity.cpp -o ./obj/inf_entity.o 
%WASI_SDK%\clang %HEADERS% %COMPILE_OPTIONS% ./source/program.cpp    -o ./obj/program.o 


rem ======= Link =======


set LINK_OPTIONS=-no-entry --allow-undefined --export=app_init --export=app_process --export=app_terminate --export=app_move --export=app_msg --export=app_button
set LIBS= %WASI_LIB%\libc.a  %WASI_LIB%\libc++abi.a   %WASI_LIB%\libc++.a 

%WASI_SDK%\wasm-ld %LINK_OPTIONS%  %LIBS% ./obj/inf_entity.o ./obj/program.o  -o ./obj/app.wasm


rem ======= Inspect =======

rem optional, convert to wat (text format) for manual inspection if difficulties arise.
rem wasm2wat available here : https://github.com/WebAssembly/wabt/releases
rem wasm2wat ./obj/app.wasm > ./obj/app.wat


rem ======= Install =======

rem push wasm binary into appropriate website directory for execution.
copy .\obj\app.wasm ..\..\website\game\_action