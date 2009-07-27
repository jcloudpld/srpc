@echo off
REM
REM NSPRC build all script
REM
REM

set ERROR=0

call build.bat build Debug 7.1
if errorlevel 1 goto build_error

call build.bat build Release 7.1
if errorlevel 1 goto build_error

call build.bat build StaticDebug 7.1
if errorlevel 1 goto build_error

call build.bat build StaticRelease 7.1
if errorlevel 1 goto build_error

if "%1" == "notproactor" goto done
if "%2" == "notproactor" goto done

call build.bat build TProactorDebug 7.1
if errorlevel 1 goto build_error

call build.bat build TProactorRelease 7.1
if errorlevel 1 goto build_error
goto done

:build_error
set ERROR=1

:done
if "%1" == "nowait" goto end
if "%2" == "nowait" goto end
pause

:end
exit /B %ERROR%

