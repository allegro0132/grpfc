# grpfc: Global complex Roots and Poles Finding in C++

## Description

`grpfc` attempts to **find all the zeros and poles of a complex valued function with complex arguments in a fixed region**.
These types of problems are frequently encountered in electromagnetics, but the algorithm can also be used for similar problems in e.g. optics, acoustics, etc.

The GRPF algorithm first samples the function on a triangular mesh through Delaunay triangulation.
Candidate regions to search for roots and poles are determined and the discretized [Cauchy's argument principle](https://en.wikipedia.org/wiki/Argument_principle) is applied _without needing the derivative of the function or integration over the contour_.
To improve the accuracy of the results, a self-adaptive mesh refinement occurs inside the identified candidate regions.

## Installation
- Pull source codes
```bash
  git clone --recurse-submodules git@github.com:allegro0132/grpfc.git
```
### Linux

- Install gcc complier
```bash
  apt install build-essential
```

### MacOS

- Install clang compiler
```bash
  brew install cmake llvm
```

### Common steps

- Prepare building directory
```bash
  mkdir build&&cd build 
```

- Prepare compile flags using cmake
```bash
  cmake ..
```

- Compile the library
```bash
  make
```

- Run the Roots and Poles solver
```bash
  ./main
```