# Hashtopolis Generic Cracker

Generic cracker example which is compatible with Hashtopolis.

This very limited MD5 cracker implementation should demonstrate how a cracker could look to provide the minimal functionality which allows to distribute it with Hashtopolis.


## Building

To build you need at least the Qt5 Core libraries installed. 

### Using qmake

Build with using qmake and make:

```
cd cracker
qmake cracker.pro
make
```

### Using cmake

Note that on Windows you need to adjust the Qt installation location in CMakeLists.txt
Build with using cmake and make:

```
mkdir build
cd build
cmake ..
make
```
