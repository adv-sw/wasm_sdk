@echo off

rem Use editor specified in EDITOR environment variable if found.

rem Or Notepad++ if found 
if not exist "%EDITOR%" ( FOR /F "tokens=2* skip=2" %%a in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\App Paths\notepad++.exe"') do ( SET EDITOR=%%b ) )

rem or Wordpad as fallback default.
if not exist "%EDITOR%" ( SET EDITOR=wordpad.exe )

start "" "%EDITOR%" %1 %2 %3 %4 %5