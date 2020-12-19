# VPX Compression Native Plugin for Unity

## License
このプロジェクトは、サードパーティのアセットを除き、MIT Licenseでライセンスされています。
This project is licensed under the MIT License excluding third party assets.

## Tested Environment
- Windows 10 Pro
- [Vcpkg 2020.04](https://github.com/microsoft/vcpkg/releases/tag/2020.04)
- [CMake 3.17.3](https://github.com/Kitware/CMake/releases/tag/v3.17.3)
- Visual Studio 2019

-----
## Setup
### Install Vcpkg
```powershell
> git clone https://github.com/microsoft/vcpkg.git --branch 2020.04
> .\vcpkg\bootstrap-vcpkg.bat
```
Although it is not required, it may be useful to set the environment variable Path.

### Install CMake
Download and execute the installer.
- https://github.com/Kitware/CMake/releases/tag/v3.17.3

### Install the English language pack in Visual Studio
- [Install Visual Studio # Step 6 - Install language packs](https://docs.microsoft.com/ja-jp/visualstudio/install/install-visual-studio?view=vs-2019#step-6---install-language-packs-optional)

-----
## How to build
1. Install libraries using vcpkg

```powershell
> vcpkg install libvpx:x86-windows libvpx:x64-windows
```

2. Run run-cmake.ps1

```powershell
> PowerShell -ExecutionPolicy RemoteSigned .\run-cmake.ps1
```

## References
