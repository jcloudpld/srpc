@echo off
REM
REM NSPRC build script
REM

set BUILD_ERROR=
set command=%1
set configname=%2
set VC_VER=%3
set SLN_FILE=msvc\nsrpc_all.sln

if "%command%"=="" goto usage
if "%configname%"=="" goto usage
if "%VC_VER%"=="" set VC_VER=7.1

:set_vcvars
setlocal
call set_vcvars.bat %VC_VER% > NUL

:build
echo BUILDING...(devenv /nologo /%command% %configname% %SLN_FILE%)
devenv /nologo /%command% %configname% %SLN_FILE%
if errorlevel 1 goto build_error

goto done

:usage
echo ---------------------------------------------
echo "USAGE: %0 <command> <config-name> [VC++ version]"
echo "  command := build | rebuild | clean"
echo "  config-name := Debug | Release | StaticDebug | StaticRelease | TProactorDebug | TProactorRelease"
echo "  VC++ version := 7.1 | 8 (default: 7.1)"
echo ---------------------------------------------
goto done

:build_error
echo *********************
echo *** Build Errors! ***
echo *********************
exit /B 1

:build_ok
echo .
echo Build succeeded!
echo .

:done
exit /B 0
