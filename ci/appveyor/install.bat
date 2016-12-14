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
    bash -lc "pacman --noconfirm --sync pacman-mirrors"
    bash -lc "pacman -S --needed --noconfirm git"

    IF %BUILDTOOL%==cmake (
        bash -lc "pacman -S --needed --noconfirm git mingw-w64-x86-cmake make"
    ) ELSE (
        echo "Unsupported build tool '%BUILDTOOL%' selected for compiler '%COMPILER%' in install script"
    )

    bash -lc "pacman -S --needed --noconfirm lcov"
    bash -lc "pacman -S --needed --noconfirm mingw-w64-x86_64-gcc"
    bash -lc "pacman -S --needed --noconfirm mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image"
    bash -lc "pacman -S --needed --noconfirm mingw-w64-x86_64-pkg-config"
) ELSE (
    echo "Unsupported compiler '%COMPILER%' selected in install script"
)

