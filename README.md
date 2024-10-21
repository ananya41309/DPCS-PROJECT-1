# Polyhedron Operations and Visualization

**Overview**  
This project provides functionality for reading, manipulating, and visualizing polyhedra. A polyhedron is defined by its vertices, edges, and faces, and the program allows users to perform various operations such as translation, rotation, slicing, visualization, and calculation of geometric properties like surface area and volume.

The program uses SDL2 for visualization and standard C libraries for file I/O and mathematical operations. It enables reading polyhedron data from a file, applying transformations, calculating properties, and saving the modified polyhedron to a file. Users can interactively choose to perform operations on the polyhedron and view the results in a graphical window.

**Features**  
- **Read/Write Polyhedron Data**: Load polyhedron data (vertices, edges, faces) from a text file and save the modified data back to a file.
- **Translate**: Move the polyhedron along the X, Y, and Z axes.
- **Rotate**: Rotate the polyhedron around the X, Y, or Z axes by a specified angle.
- **Slice**: Slice the polyhedron into two parts using a user-defined plane.
- **Visualize**: Render the polyhedron in a 3D space using SDL2 with perspective projection.
- **Geometric Properties**: Calculate and display the surface area and volume of the polyhedron.

**Requirements**  
- **SDL2**: Simple DirectMedia Layer (SDL) for rendering and visualization.
  - **Linux**: Install via package manager (`sudo apt-get install libsdl2-dev`).
  - **Windows**: Download and install SDL2 from [SDL's official site](https://www.libsdl.org/).
- **C Compiler**: Such as `gcc`.

**File Structure**  
- **Vertex**: Struct for storing the 3D coordinates (x, y, z) of a vertex.
- **Edge**: Struct representing an edge between two vertices (`v1`, `v2`).
- **Face**: Struct for representing a polygonal face, defined by a set of vertices.
- **Polyhedron**: Struct containing arrays of vertices, edges, and faces, along with the respective counts.

**Operations**  
- **Translation**: Translate the polyhedron uniformly along the X, Y, and Z axes.
- **Rotation**: Rotate the polyhedron around the X, Y, or Z axes about its centroid by a specified angle (in degrees).
- **Slicing**: Slice the polyhedron using a plane defined by the equation `Ax + By + Cz + D = 0`, creating two new polyhedra.
- **Visualization**: Render the polyhedron as wireframes in a 3D perspective view using SDL2.
- **Geometric Properties**: Calculate the surface area and volume of the polyhedron based on its vertices and faces.

**Input and Output**  
- **Input Format**:  
  The input file must be structured as follows:
  ```
  Vertex Count: N
  Edge Count: M
  Face Count: F
  x1 y1 z1
  x2 y2 z2
  ...
  v1 v2
  v3 v4
  ...
  f1 f2 f3 ...
  ```
  - `N`: Number of vertices; each subsequent line contains the coordinates (x, y, z) of a vertex.
  - `M`: Number of edges; each line after that represents an edge between two vertices (indexed by their number in the vertex list).
  - `F`: Number of faces; each line defines a face by listing the indices of the vertices that form it.

- **Output Files**:  
  The program generates output files after translation, rotation, slicing, or geometric calculations, storing the updated polyhedron's data in the same format as the input file.

**Usage**  
- **Compile the Program**:  
  ```bash
  gcc polyhedron.c -o polyhedron -lSDL2 -lm
  ```
- **Run the Program**:  
  ```bash
  ./polyhedron
  ```
  You will be prompted to enter the input file name and choose from the available operations (translation, rotation, slicing, calculation of geometric properties, or exit).

**Example Input File**  
```
Vertex Count: 4
Edge Count: 6
Face Count: 4
0 0 0
1 0 0
0 1 0
0 0 1
0 1
1 2
2 0
0 2
0 3
2 3
3 0 1
```

**Example Commands**  
- **Translation**:  
  ```
  Choose operation: t
  Enter translation values (dx dy dz): 1 1 1
  ```
- **Rotation**:  
  ```
  Choose operation: r
  Enter axis for rotation (x, y, z): x
  Enter rotation angle (degrees): 90
  ```
- **Slicing**:  
  ```
  Choose operation: s
  Enter the normal vector of the slicing plane (A B C): 1 0 0
  Enter the distance from the origin (D): -0.5
  ```
- **Calculate Geometric Properties**:  
  ```
  Choose operation: g
  Surface area: 4.0
  Volume: 1.333
  ```

**Visualization**  
After performing a translation or rotation, the polyhedron is visualized in a 640x480 window. The program projects the 3D coordinates onto a 2D screen using simple perspective projection.

**Future Improvements**  
- **Advanced Rendering**: Add more advanced rendering techniques, such as shading or more realistic 3D rendering.
- **Improved Slicing**: Implement full functionality for slicing the polyhedron into two separate parts and handling face and edge reconstruction.
- **User Interface**: Create a more user-friendly interface for choosing operations and inputting parameters.
