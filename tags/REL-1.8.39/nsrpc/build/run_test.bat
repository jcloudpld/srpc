@echo off
REM
REM Run SRPC test script
REM

set exename=nsrpcTest
set configname=%1

if "%configname%"=="Debug" goto debug
if "%configname%"=="TProactorDebug" goto tpdebug
if "%configname%"=="TProactorRelease" goto tprelease
if "%configname%"=="Release" goto test
goto usage

:debug
set exename=%exename%_d
goto test

:tpdebug
set exename=%exename%_tp_d
goto test

:tprelease
set exename=%exename%_tp
goto test

:test
pushd .
cd ../bin
%exename%

goto done

:usage
echo ---------------------------------------------
echo "USAGE: %0 <config-name>"
echo "  config-name := Debug | Release | TProactorDebug | TProactorRelease"
echo ---------------------------------------------

:done
popd
