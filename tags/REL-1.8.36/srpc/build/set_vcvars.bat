@echo off

set VC_VER=%1

if "%VC_VER%"=="7.1" goto set_vc71vars
if "%VC_VER%"=="8" goto set_vc8vars

echo INVALID VC++ Version: %VC_VER%
goto done

:set_vc71vars
echo USING VC7.1...
"%VS71COMNTOOLS%vsvars32.bat"
goto done

:set_vc8vars
echo USING VC8...
"%VS80COMNTOOLS%vsvars32.bat"
goto done

:done

