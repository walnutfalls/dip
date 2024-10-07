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

The application state has 3 images: A, B, OUTPUT. Operations require either 1 or 2 operands.
For one operand operations, A is the source.
Results are output always to OUTPUT. 

There's going to be a Operands and Output views, where  operands shows a split screen view of A and B.

The output view will show OUTPUT. 

### Controls (PP1)

This will have various controls for performing operations for A and B, and writing the result to output. 
It will also Have controls for switching views. 

### Console (PP1)

This is a custom console with a bunch of commands available.

`cd`, `ls` are implemented. Tab completion has very basic implementation.

```
[load|save] <ppm_path>"
[add|sum|mul] -i <ppm_path1> <ppm_path2> -i <out_ppm_path>
inv -i <ppm_path1> -o <out_ppm_path>
log -i <ppm_path1> -o <out_ppm_path> -c <const> -b <base>
pow -i <ppm_path1> -o <out_ppm_path> -c <const> -gamma <gamma>
 ```

### Histograms (PP2)

Can manually equalize A and B, and do a histogram match from A to B.

#### Filling the gap in G_inv

In order to perform histogram patching, we calculate the cumulative T table for texture B,
which is referred to as G in the book. Then, we calcualte the inverse function - all the
[key, value] pairs in G become [value, key] paris in G_inv. 

At the end of this process, there are a lot of intensities which are missing as values from G_inv. 

If nothing is done, there is noise scattered throughout the result image.

In this app, we interpolate linearly between values in the lookup. After computing G_inv, 
we loop through it and replace any zeroes like this:

```
// find the nearest mapping, and interpolate
//| 4 | 0 | 0 | 0 | 0 | 9 |
//  0   1  ^2   3   4   5
// range = 5-0=5, 
// weigh non-0 neighbors by distance from i: (1-2/5)*4 + (1-3/5) * 9
```



### Filters

This is a UI for blur and smoothing

```
Filter->Blur->Gaussian Blur
```


## Platforms
Windows. 
Was developed using 2022 Visual Studio toolkit, tested on 2019. 
### What needs to be ported?
#### `/src/os/browse_dialog.cpp`
Implementation for the native file browse dialog on windows. 

It's possible (looks like tricky on osx) to add implementations for osx/linux, or to just use a library from  github.

A basic file browser can also be implemented in IMGui.