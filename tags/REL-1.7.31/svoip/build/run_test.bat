@echo off
REM
REM Run SVOIP test script
REM

set exename=svoipTest
set configname=%1

if "%configname%"=="Debug" goto debug
if "%configname%"=="Release" goto test
goto usage

:debug
set exename=%exename%_d

:test
pushd .
cd ../bin
%exename%

goto done

:usage
echo ---------------------------------------------
echo "USAGE: %0 <config-name>"
echo "  config-name := Debug | Release"
echo ---------------------------------------------

:done
popd
