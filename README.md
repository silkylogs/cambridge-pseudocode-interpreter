# cambridge-pseudocode-interpreter
An interpreter for a programming language conforming to the
Cambridge A level computer science teacher's pseudocode guide

## Build instructions
### Windows
```bat
:: Setting up build environment:
premake5 vs2019

:: Compiling:
msbuild CambridgePseudocodeInterpeter.sln
```

### Linux, with clang
```sh
# Setting up build environment:
premake5 gmake

# Compile
make config=linux-clang
```

### Linux, with gcc
```sh
# Setting up build environment:
premake5 gmake

# Compile
make config=linux-gcc
```