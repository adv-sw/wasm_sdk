rem @echo off

rem windoze copy command doesn't support / path delimitors so switch to supported format here.


set FIX_1=%1
set FIX_2=%2

set "FIX_1=%FIX_1:/=\%"
set "FIX_2=%FIX_2:/=\%"

rem echo copy %FIX_1% %FIX_2%
copy %FIX_1% %FIX_2% > nul