param(
    [Parameter(Mandatory=$true)][string]$CubeCltRoot,
    [Parameter(Mandatory=$true)][string]$CubeG4Root,
    [ValidateSet('phase3', 'phase5', 'diag', 'all')][string]$Target = 'phase3'
)
$ErrorActionPreference = 'Stop'
$cmake = Join-Path $CubeCltRoot 'CMake/bin/cmake.exe'
$ninja = Join-Path $CubeCltRoot 'Ninja/bin/ninja.exe'

if ($Target -eq 'phase3' -or $Target -eq 'all') {
    Write-Host "=== Building Phase 3 Baseline (tickform_phase3_xo) ===" -ForegroundColor Cyan
    $build3 = Join-Path $PSScriptRoot 'build/phase3'
    & $cmake --fresh -S $PSScriptRoot -B $build3 -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" "-DCMAKE_TOOLCHAIN_FILE=$PSScriptRoot/cmake/arm-gcc.cmake" "-DSTM32_CLT_ROOT=$CubeCltRoot" "-DSTM32_CUBE_G4_ROOT=$CubeG4Root"
    if ($LASTEXITCODE -ne 0) { throw 'CMake configuration for Phase 3 failed' }
    & $cmake --build $build3 --target tickform_phase3_xo --clean-first
    if ($LASTEXITCODE -ne 0) { throw 'Firmware build for Phase 3 failed' }
}

if ($Target -eq 'phase5' -or $Target -eq 'all') {
    Write-Host "=== Building Phase 5 Variant (tickform_phase5_i2s_rx) ===" -ForegroundColor Cyan
    $build5 = Join-Path $PSScriptRoot 'build/phase5'
    & $cmake --fresh -S $PSScriptRoot -B $build5 -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" "-DCMAKE_TOOLCHAIN_FILE=$PSScriptRoot/cmake/arm-gcc.cmake" "-DSTM32_CLT_ROOT=$CubeCltRoot" "-DSTM32_CUBE_G4_ROOT=$CubeG4Root"
    if ($LASTEXITCODE -ne 0) { throw 'CMake configuration for Phase 5 failed' }
    & $cmake --build $build5 --target tickform_phase5_i2s_rx --clean-first
    if ($LASTEXITCODE -ne 0) { throw 'Firmware build for Phase 5 failed' }
}

if ($Target -eq 'diag' -or $Target -eq 'all') {
    Write-Host "=== Building Phase 5 Diagnostic (tickform_phase5_diag) ===" -ForegroundColor Cyan
    $buildDiag = Join-Path $PSScriptRoot 'build/diag'
    & $cmake --fresh -S $PSScriptRoot -B $buildDiag -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" "-DCMAKE_TOOLCHAIN_FILE=$PSScriptRoot/cmake/arm-gcc.cmake" "-DSTM32_CLT_ROOT=$CubeCltRoot" "-DSTM32_CUBE_G4_ROOT=$CubeG4Root"
    if ($LASTEXITCODE -ne 0) { throw 'CMake configuration for Diagnostic failed' }
    & $cmake --build $buildDiag --target tickform_phase5_diag --clean-first
    if ($LASTEXITCODE -ne 0) { throw 'Firmware build for Diagnostic failed' }
}
