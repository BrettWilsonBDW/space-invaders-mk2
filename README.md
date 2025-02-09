# space-invaders-mk2

A improved version of [space-invaders-clone](https://github.com/BrettWilsonBDW/space-invaders-clone) made using [SDL3](https://www.libsdl.org/)

## wasm build

Play the game in the browser [here](https://brettwilsonbdw.github.io/space-invaders-mk2/)

# Building from Source

## Prerequisites
- [cmake](https://cmake.org/)
- [mingw](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/) or [Visual Studio](https://visualstudio.microsoft.com/)
- [Emscripten](https://github.com/emscripten-core/emsdk) (for web build)

## Building for Desktop

```bash
## Clone the repository
git clone https://github.com/BrettWilsonBDW/space-invaders-mk2.git

## Navigate to the project directory
cd space-invaders-mk2

## Create a build directory
mkdir build

## Navigate to the build directory
cd build

## Run CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

## Build the project using Visual Studio (you can also use Ninja or MinGW)
cmake --build . --config Release
```
## Building for wasm with Emscripten

```bash
## Ensure Emscripten is installed and configured:
## on windows
cd C:\
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
.\emsdk.bat install latest
.\emsdk.bat activate latest --permanent

## Navigate to the project directory
cd space-invaders-mk2

## Navigate to the tools directory
cd tools

## Run the build script for the web
.\build-sdl3-emcc.bat
```

## Acknowledgments

- [SDL3](https://www.libsdl.org/) - For windowing and rendering.