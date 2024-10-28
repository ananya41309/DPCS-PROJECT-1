#ifndef POLY_OPERATIONS_H
#define POLY_OPERATIONS_H

#include "data_structures.h"


void translate_polyhedron(Polyhedron *p, float dx, float dy, float dz);
void rotate_polyhedron_x(Polyhedron *p, float angle);
void rotate_polyhedron_y(Polyhedron *p, float angle);
void rotate_polyhedron_z(Polyhedron *p, float angle);
void slice_polyhedron(Polyhedron *p, float A, float B, float C, float D, Polyhedron **part1, Polyhedron **part2);
float tetrahedron_volume(Vertex v0, Vertex v1, Vertex v2, Vertex v3);
float calculate_volume(Polyhedron *p);
Vertex calculate_centroid(Polyhedron *p);
Vertex cross_product(Vertex v1, Vertex v2);
float vector_magnitude(Vertex v);
float polygon_area(Polyhedron *p, Face face);
float calculate_surface_area(Polyhedron *p);
void project_front_view(Polyhedron *p);
void project_top_view(Polyhedron *p);
void project_side_view(Polyhedron *p);
void extract_vertices_from_projection(Vertex *vertices, int vertex_count, char view);
Polyhedron* reconstruct_polyhedron_from_views(Vertex *front_view, int front_count, Vertex *top_view, int top_count, Vertex *side_view, int side_count);
int find_or_create_vertex(Vertex v, Vertex *reconstructed_vertices, int *reconstructed_count);
int find_matching_vertex(float x, float y, float z, ReconstructedVertex *vertices, int count);
#endif
