@echo off
rem silence console output & start debugger
start /b cmd /c start /min ..\..\tools\lldbg -- infinity.exe #ui=0 #desktop #g ..\..\website\simple 