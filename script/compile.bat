SET WORKSPACE=%~dp0..

SET BUILD_DIR=%WORKSPACE%\build

cmake -S %WORKSPACE% -B %BUILD_DIR% -DCMAKE_TOOLCHAIN_FILE=%WORKSPACE%\sdk\win\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build %BUILD_DIR% 