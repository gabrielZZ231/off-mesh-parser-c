# off-mesh-parser-c

A C program that parses 3D mesh files in the Object File Format (.off) using a Face-Vertex data structure, and exports 2D SVG projections of the meshes.

## Files
- `malha_face_vertice.c`: The main source code. It reads the `.off` files and exports them to `.svg`.
- `triangles.off`: A sample mesh file containing a simple triangular geometry.
- `hand-hybrid.off`: A sample mesh file containing a more complex geometry of a hand.
- `triangles.svg`: Generated 2D projection of `triangles.off`.
- `hand-hybrid.svg`: Generated 2D projection of `hand-hybrid.off`.

## How to Compile and Run
Use GCC or any standard C compiler to compile the program:
```bash
gcc malha_face_vertice.c -o malha
```

Run the program:
```bash
./malha
```
(On Windows: `malha.exe`)

The program will automatically load `triangles.off` and `hand-hybrid.off`, print a summary of their vertices and faces, and generate the corresponding `.svg` files.
