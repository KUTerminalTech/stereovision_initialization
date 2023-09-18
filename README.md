# Prerequisite libraries

* cmake
* OpenCV
* fmt
* yaml-cpp
* tinyxml2

# Clone repository

``` bash
$ git clone --depth 1 https://github.com/KUTerminalTech/stereovision_initialization.git
```

# build & compile

## Build with CMake

### POSIX-compliant operating system (Include Linux and Apple macOS)

``` bash
$ cmake -B build -DCMAKE_BUILD_TYPE=Release
$ cd build
$ make -j2
```

### Apple macOS with XCode project (only using xcode)

``` bash
$ cmake -B build -G Xcode
```

### Microsoft Window with MinGW cross-compilation

``` bash
$ cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-x86_64-w64-mingw32.cmake # replace with your toolchain file
$ cd build
$ make -j2
```

### Microsoft Windows with Microsoft Visual C++

``` bash
$ cmake -B build -G "NMake Makefiles"
$ cd build
$ nmake
```