# VPX Compression Native Plugin for Unity

## License
このプロジェクトは、サードパーティのアセットを除き、MIT Licenseでライセンスされています。
This project is licensed under the MIT License excluding third party assets.

## Tested Environment
- Windows 10 Pro
- [Vcpkg 2020.04](https://github.com/microsoft/vcpkg/releases/tag/2020.04)
- [CMake 3.17.3](https://github.com/Kitware/CMake/releases/tag/v3.17.3)
- Visual Studio 2019

## How to build
1. Install libraries using vcpkg

```powershell
vcpkg install libvpx:x86-windows libvpx:x64-windows
```

2. Run run-cmake.ps1

```powershell
PowerShell -ExecutionPolicy RemoteSigned .\run-cmake.ps1
```

## References
