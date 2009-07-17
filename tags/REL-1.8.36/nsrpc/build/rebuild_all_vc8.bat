@echo off
REM
REM SPRC rebuild all script
REM

set ERROR=0
call build.bat rebuild Debug 8
if errorlevel 1 goto build_error

call build.bat rebuild Release 8
if errorlevel 1 goto build_error

call build.bat rebuild StaticDebug 8
if errorlevel 1 goto build_error

call build.bat rebuild StaticRelease 8
if errorlevel 1 goto build_error

if "%1" == "notproactor" goto done
if "%2" == "notproactor" goto done

call build.bat rebuild TProactorDebug 8
if errorlevel 1 goto build_error

call build.bat rebuild TProactorRelease 8
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
