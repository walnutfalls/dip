# Technologies Being Used
This is a CMake project, using [vcpkg]([microsoft/vcpkg: C++ Library Manager for Windows, Linux, and MacOS (github.com)](https://github.com/microsoft/vcpkg)) for dependency management. See `vcpkg.json` for a list of libraries that this project depends on. 
## Building
`Build.bat` in the root directory should product a `build/dip.exe` program. 

This script:
* Downloads and bootstraps vcpkg into /bin/vcpkg
* Runs `vcvarsall.bat` on a detected visual studio install, thereby setting the environment for CMake. Then, hopefully CMake will pick up the right compiler. 
* Performs CMake configure step, which should use the vcpkg toolchain and install all the dependencies from the internet. **This may take a while - it's going to build opencv and glfw**.
* Runs build
### Prerequisites
* Visual studio version 2022 or 2019 with C++ features installed.
* C++17
* `Build.bat` tries to use 2022, 2019 or 2017. Anything older assuming won't work.
## Features
Please expand the ImGui panels in the upper left corner of the app! They
start out small, but are draggable and sizeable. 

There is a controls panel with all the operations related controls. 


There is a console panel with the basic operations outline in assignment.

```
cd ~/Test-images
add -i cup-12.ppm beetle-13.ppm -o output.ppm
```

## Platforms
Windows. 
Was developed using 2022 Visual Studio toolkit, tested on 2019. 
### What needs to be ported?
#### `/src/os/browse_dialog.cpp`
Implementation for the native file browse dialog on windows. 

It's possible (looks like tricky on osx) to add implementations for osx/linux, or to just use a library from  github.

A basic file browser can also be implemented in IMGui.