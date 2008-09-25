@echo off
REM
REM SPRC build all script
REM

set ERROR=0

call build.bat build Debug 8
if errorlevel 1 goto build_error

call build.bat build Release 8
if errorlevel 1 goto build_error

call build.bat build StaticDebug 8
if errorlevel 1 goto build_error

call build.bat build StaticRelease 8
if errorlevel 1 goto build_error
goto done

:build_error
set ERROR=1

:done
if "%1" == "nowait" goto end
pause

:end
exit /B %ERROR%
