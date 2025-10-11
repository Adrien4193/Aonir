# C++ template project

My C++ template project (Windows only for now).

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

## VSCode

Example settings.json to have:

- A custom terminal with Visual Studio Developer PowerShell.
- Visual studio CMake plugin able to find Visual Studio compile tools.
- Ignore GoogleTest warnings with clang-tidy.

```json
{
    "terminal.integrated.profiles.windows": {
        "Developer PowerShell": {
            "source": "PowerShell",
            "args": [
                "-NoExit",
                "-Command",
                "& \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\Launch-VsDevShell.ps1\" -Arch amd64"
            ]
        }
    },
    "cmake.useVsDeveloperEnvironment": "always",
    "C_Cpp.codeAnalysis.exclude": {
        "Tests/": true
    }
}
```

Example tasks.json to build with CMake using Ctrl+Shift+B:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cmake",
            "label": "CMake: build",
            "command": "build",
            "preset": "${command:cmake.activeBuildPresetName}",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [],
            "detail": "CMake template build task"
        }
    ]
}
```

Example launch.json to debug apps and tests:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug application",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${command:cmake.launchTargetPath}",
            "cwd": "${workspaceFolder}",
            "console": "integratedTerminal"
        },
        {
            "name": "(ctest) Launch",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${cmake.testProgram}",
            "cwd": "${workspaceFolder}",
            "args": [
                "${cmake.testArgs}"
            ]
        }
    ]
}
```

## User presets

For convenience, it is easier to set toolchain file, build type or compiler in a CMakeUserPresets.json at the root of the repo:

```json
{
    "version": 10,
    "configurePresets": [
        {
            "name": "user",
            "hidden": true,
            "inherits": "windows",
            "toolchainFile": "${sourceDir}/../vcpkg/scripts/buildsystems/vcpkg.cmake",
            "cacheVariables": {
                "CMAKE_CXX_COMPILER": "cl"
            }
        },
        {
            "name": "debug",
            "inherits": "user",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug"
            }
        },
        {
            "name": "release",
            "inherits": "user",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release"
            }
        }
    ],
    "buildPresets": [
        {
            "name": "debug",
            "inherits": "windows",
            "configurePreset": "debug"
        },
        {
            "name": "release",
            "inherits": "windows",
            "configurePreset": "release"
        }
    ],
    "testPresets": [
        {
            "name": "debug",
            "inherits": "windows",
            "configurePreset": "debug"
        },
        {
            "name": "release",
            "inherits": "windows",
            "configurePreset": "release"
        }
    ],
    "packagePresets": [
        {
            "name": "debug",
            "inherits": "windows",
            "configurePreset": "debug"
        },
        {
            "name": "release",
            "inherits": "windows",
            "configurePreset": "release"
        }
    ],
    "workflowPresets": [
        {
            "name": "debug",
            "steps": [
                {
                    "type": "configure",
                    "name": "debug"
                },
                {
                    "type": "build",
                    "name": "debug"
                },
                {
                    "type": "test",
                    "name": "debug"
                },
                {
                    "type": "package",
                    "name": "debug"
                }
            ]
        },
        {
            "name": "release",
            "steps": [
                {
                    "type": "configure",
                    "name": "release"
                },
                {
                    "type": "build",
                    "name": "release"
                },
                {
                    "type": "test",
                    "name": "release"
                },
                {
                    "type": "package",
                    "name": "release"
                }
            ]
        }
    ]
}
```
