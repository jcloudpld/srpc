@echo off

echo Unit Testing...

pushd .
cd build

echo ==== Debug Version ====
call run_test.bat Debug

echo ==== Release Version ====
call run_test.bat Release

echo ==== TProactorDebug Version ====
call run_test.bat TProactorDebug

echo ==== TProactorRelease Version ====
call run_test.bat TProactorRelease
popd

pause
