# 3D Renderer (Low-Level)

A simple **software 3D renderer in C**, built to explore how graphics work at the lowest level.  
No OpenGL, no GPU pipeline — just math, pixels, and SDL2.

## Features
- Software framebuffer
- Vector & matrix math
- Perspective projection
- Triangle rendering / wireframe
- SDL2 window output

## Goals
- Understand 3D transformations
- Learn how projection works
- Implement rasterization manually
- Build a small software rendering pipeline

## Build
Requires **SDL2 v2.0.12**.

```bash
  gcc main.c -o 3drenderer -lSDL2
```

Run:
```bash
  ./3drenderer
```

## Purpose
A small project to learn low-level graphics by implementing everything manually.