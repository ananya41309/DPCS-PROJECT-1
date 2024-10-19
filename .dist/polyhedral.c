#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//Data Structure
// Define a 3D vector (vertex)
typedef struct {
    float x, y, z;
} Vertex;

// Define an Edge between two vertices
typedef struct {
    int v1, v2;  // Indices of vertices
} Edge;

// Define a Face (Polygon with n vertices)
typedef struct {
    int *vertices;  // Array of vertex indices
    int vertex_count;
} Face;

// Define a Polyhedron consisting of vertices, edges, and faces
typedef struct {
    Vertex *vertices;
    int vertex_count;
    
    Edge *edges;
    int edge_count;
    
    Face *faces;
    int face_count;
} Polyhedron;

// Function to create a polyhedron
Polyhedron* create_polyhedron(int vertex_count, int edge_count, int face_count) {
    Polyhedron *p = (Polyhedron*)malloc(sizeof(Polyhedron));
    p->vertices = (Vertex*)malloc(vertex_count * sizeof(Vertex));
    p->edges = (Edge*)malloc(edge_count * sizeof(Edge));
    p->faces = (Face*)malloc(face_count * sizeof(Face));
    p->vertex_count = vertex_count;
    p->edge_count = edge_count;
    p->face_count = face_count;
    return p;
}

// Free the allocated memory
void free_polyhedron(Polyhedron *p) {
    free(p->vertices);
    free(p->edges);
    for (int i = 0; i < p->face_count; i++) {
        free(p->faces[i].vertices);
    }
    free(p->faces);
    free(p);
}

//input

void input_vertices(Polyhedron *p) {
    printf("Enter the coordinates of %d vertices (x y z):\n", p->vertex_count);
    for (int i = 0; i < p->vertex_count; i++) {
        printf("Vertex %d: ", i + 1);
        scanf("%f %f %f", &p->vertices[i].x, &p->vertices[i].y, &p->vertices[i].z);
    }
}

void input_edges(Polyhedron *p) {
    printf("Enter the %d edges (vertex1_index vertex2_index):\n", p->edge_count);
    for (int i = 0; i < p->edge_count; i++) {
        printf("Edge %d: ", i + 1);
        scanf("%d %d", &p->edges[i].v1, &p->edges[i].v2);
    }
}

void input_faces(Polyhedron *p) {
    for (int i = 0; i < p->face_count; i++) {
        printf("Enter the number of vertices for face %d: ", i + 1);
        scanf("%d", &p->faces[i].vertex_count);
        p->faces[i].vertices = (int*)malloc(p->faces[i].vertex_count * sizeof(int));
        printf("Enter the vertices for face %d: ", i + 1);
        for (int j = 0; j < p->faces[i].vertex_count; j++) {
            scanf("%d", &p->faces[i].vertices[j]);
        }
    }
}

//translate
void translate(Vertex vertices[], int num_vertices, float tx, float ty, float tz) {
    for (int i = 0; i < num_vertices; i++) {
        // Apply translation
        vertices[i].x += tx;
        vertices[i].y += ty;
        vertices[i].z += tz;
    }
}

void translate_polyhedron(Polyhedron *p, float dx, float dy, float dz) {
    for (int i = 0; i < p->vertex_count; i++) {
        p->vertices[i].x += dx;
        p->vertices[i].y += dy;
        p->vertices[i].z += dz;
    }
    printf("Polyhedron translated by (%f, %f, %f)\n", dx, dy, dz);
}

//rotation


void rotate_polyhedron_z(Polyhedron *p, float angle) {
    float radians = angle * M_PI / 180.0;
    for (int i = 0; i < p->vertex_count; i++) {
        float x_new = p->vertices[i].x * cos(radians) - p->vertices[i].y * sin(radians);
        float y_new = p->vertices[i].x * sin(radians) + p->vertices[i].y * cos(radians);
        p->vertices[i].x = x_new;
        p->vertices[i].y = y_new;
    }
    printf("Polyhedron rotated around Z-axis by %f degrees\n", angle);
}

//visualize


