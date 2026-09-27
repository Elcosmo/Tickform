param(
    [Parameter(Mandatory=$true)][string]$CubeCltRoot,
    [Parameter(Mandatory=$true)][string]$CubeG4Root
)
$ErrorActionPreference = 'Stop'
$cmake = Join-Path $CubeCltRoot 'CMake/bin/cmake.exe'
$ninja = Join-Path $CubeCltRoot 'Ninja/bin/ninja.exe'
$build = Join-Path $PSScriptRoot 'build/phase2'
& $cmake --fresh -S $PSScriptRoot -B $build -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" "-DCMAKE_TOOLCHAIN_FILE=$PSScriptRoot/cmake/arm-gcc.cmake" "-DSTM32_CLT_ROOT=$CubeCltRoot" "-DSTM32_CUBE_G4_ROOT=$CubeG4Root"
if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed' }
& $cmake --build $build --clean-first
if ($LASTEXITCODE -ne 0) { throw 'Firmware build failed' }
