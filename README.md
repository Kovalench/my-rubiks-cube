# Rubik's Cube 3D Simulation (C++ / Raylib)

This project is a 3D Rubik’s Cube simulation built using **C++** and **Raylib**.  
It renders a fully interactive Rubik’s Cube, handles real-time face rotations, manages color mapping, and uses 3D transformations to preserve the cube's logical structure. The project emphasizes mathematical accuracy, performance, and clean implementation of rotation mechanics.

---

## Features
- Full 3D rendering of a Rubik’s Cube
- Accurate face rotation logic with proper cubelet remapping
- Interactive camera using mouse input
- Smooth animations for cube turns
- Configurable key bindings for all face rotations
- Lightweight implementation using Raylib

---

## Controls
- **Right mouse drag** – rotate camera  
- **Q / W / E / A / S / D** – rotate cube faces  
- **Hold SHIFT** – invert rotation direction  
- **ESC** – exit

---

## Project Structure

/src
Cube.cpp # main source file (rendering + rotations)

/include # optional headers
/assets # screenshots or GIFs
CMakeLists.txt
README.md

---

## Build Instructions

### Requirements
- C++17 or later  
- CMake 3.10+  
- Raylib library  

### Build on Linux / macOS / Windows (MinGW or MSVC)
```bash
mkdir build
cd build
cmake ..
cmake --build .
./rubiks_cube
