@echo off
cd source
rem we start lldbg like this to silence console output
start /b cmd /c start /min ..\..\..\tools\lldbg -- infinity.exe #ui=0 #desktop #g ..\..\..\website\particles 
cd ..
