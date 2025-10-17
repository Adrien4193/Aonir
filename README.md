# C++ game engine

My C++ game engine (Windows only for now).

## Requirements

- [Git](https://git-scm.com/downloads).
- [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/#remote-tools-for-visual-studio-2022).
- [CMake](https://cmake.org/download).
- [Ninja](https://ninja-build.org).
- [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started) Optional, dependencies can be provided by other means.
- [NSIS](https://nsis.sourceforge.io/Download) Optional, only to package an installer on Windows.

## Building

If you don't use vcpkg, make sure all CMake dependencies listed in [vcpkg.json](vcpkg.json) are available in PATH.

If you use vcpkg, it will fetch the dependencies automatically when configuring CMake if you provide the correct toolchain file (see below).

See [CMakePresets.json](CMakePresets.json) for available presets or run `cmake --list-presets`.

First setup some CMake variables (for example windows debug with vcpkg):

```bash
export $CMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
export $CMAKE_CXX_COMPILER=g++
export $CMAKE_BUILD_TYPE=Debug
```

Or:

```powershell
$Env:CMAKE_TOOLCHAIN_FILE = "path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
$Env:CMAKE_CXX_COMPILER = "g++"
$Env:CMAKE_BUILD_TYPE = "Debug"
```

Then run CMake:

```bash
cmake . --preset windows
cmake --build --preset windows
```

## Testing

```shell
ctest --preset windows
```

## Packaging

```shell
cpack --preset windows
```

## Workflows

```shell
cmake --workflow --preset windows
```
