@echo off
REM
REM SPRC rebuild all script
REM

set ERROR=0

call build.bat rebuild Debug 7.1
if errorlevel 1 goto build_error

call build.bat rebuild Release 7.1
if errorlevel 1 goto build_error

call build.bat rebuild StaticDebug 7.1
if errorlevel 1 goto build_error

call build.bat rebuild StaticRelease 7.1
if errorlevel 1 goto build_error
goto done

:build_error
set ERROR=1

:done
if "%1" == "nowait" goto end
pause

:end
exit /B %ERROR%

