# Hashtopolis Bcrypt Generic Cracker

[![Build Status](https://travis-ci.org/s3inlc/hashtopolis-generic-cracker.svg?branch=bcrypt)](https://travis-ci.org/s3inlc/hashtopolis-generic-cracker)

Generic cracker example which is compatible with Hashtopolis.

Slightly adjusted from the main version to be able to crack bcrypt hashes. Bcrypt implementation is taken from https://github.com/trusch/libbcrypt

**Note**: The bcrypt implementation seems to have a problem with compiling on Windows, but it works on Linux and OS X.


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

```
mkdir build
cd build
cmake ..
make
```
