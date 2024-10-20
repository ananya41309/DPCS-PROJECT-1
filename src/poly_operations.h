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
Vertex cross_product(Vertex v1, Vertex v2);
Vertex calculate_centroid(Polyhedron *p);
float vector_magnitude(Vertex v);
float polygon_area(Polyhedron *p, Face face);
float calculate_surface_area(Polyhedron *p);
#endif
