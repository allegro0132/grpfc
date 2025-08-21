# GESSIE: Electrostatic solver for quantum devices with autograd

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