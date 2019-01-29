# TestNet

## Description
This is a very simple neural network I put together so I could make sure I understood how nn's worked, and also observe the differences in learning between a few of the activation functions.

## Dependency Instructions
This project requires installation of the following libaries:
 * [SDL2](https://wiki.libsdl.org/Tutorials)
 * [SDL TTF](https://www.libsdl.org/projects/SDL_ttf/)

### Ubuntu 18.04
```bash
 sudo apt-get install libsdl2-dev libsdl-ttf2.0-dev
```

## Building and Running - CMake Instructions
The building of the project is done using CMake.

```bash
mkdir build
cd build
cmake ..
make
./nntest
```

## General Comments & Features
The activation functions it uses are ReLU, Sigmoid, and Tanh, and the data sets it can train on are cos, sin, and xor. When you select a new activation function or data set, the current network will be discarded, and a new one will be created with specifications that optimized the learning so that it ran at a decent framerate.The visualization just shows the neural network's attampt at matching the input function; for sin and cos this is a blue line trying to match up with a black line, with xor the output becomes grayscale, with the corners being 0,0 in the upper left, and 1,1 in the loewr right.


