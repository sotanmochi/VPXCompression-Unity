$VcpkgPath = (gcm vcpkg).Definition
$VcpkgPath = Split-Path -Parent $VcpkgPath

$CMakeToolchainFile = $VcpkgPath + "\scripts\buildsystems\vcpkg.cmake"

Write-Output "Vcpkg path:  '$VcpkgPath'"
Write-Output "CMakeToolchainFile:  '$CMakeToolchainFile'"

cmake -S . -B .\build\x64 -G "Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE="$CMakeToolchainFile"
