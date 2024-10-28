#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    int v1, v2;
} Edge;

typedef struct {
    int *vertices;
    int vertex_count;
} Face;

typedef struct {
    Vertex *vertices;
    int vertex_count;
    Edge *edges;
    int edge_count;
    Face *faces;
    int face_count;
} Polyhedron;

typedef struct {
    float x, y, z;
    int matched;  // Flag to mark if the vertex has been matched across projections
} ReconstructedVertex;

Polyhedron* create_polyhedron(int vertex_count, int edge_count, int face_count);
void free_polyhedron(Polyhedron *p);

#endif
