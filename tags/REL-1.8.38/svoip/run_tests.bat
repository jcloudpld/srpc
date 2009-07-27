@echo off

echo Unit Testing...

pushd .
cd build

echo ==== Debug Version ====
call run_test.bat Debug

echo ==== Release Version ====
call run_test.bat Release

popd

pause
