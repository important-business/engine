@echo off
cd %APPVEYOR_BUILD_FOLDER%

echo Compiler: %COMPILER%
echo Architecture: %MSYS2_ARCH%
echo Platform: %PLATFORM%
echo MSYS2 directory: %MSYS2_DIR%
echo MSYS2 system: %MSYSTEM%
echo Configuration: %CONFIGURATION%
echo Bits: %BIT%

IF %COMPILER%==msys2 (
    @echo on
    SET "PATH=C:\%MSYS2_DIR%\%MSYSTEM%\bin;C:\%MSYS2_DIR%\usr\bin;%PATH%"
    IF %BUILDTOOL%==cmake (
        echo "Using build tool 'cmake'..."
        bash -lc "cd ${APPVEYOR_BUILD_FOLDER} && exec 0</dev/null && cd build && CXX=g++ CC=gcc cmake -G 'MSYS Makefiles' .. && cmake -LAH .. && make engine build_test test"
    ) ELSE (
        echo "Unsupported build tool '%BUILDTOOL%' selected for compiler '%COMPILER%' in build script"
    )
) ELSE (
    echo "Unsupported compiler selected for build script %COMPILER%"
)

