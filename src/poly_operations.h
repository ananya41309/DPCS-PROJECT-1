#ifndef POLY_OPERATIONS_H
#define POLY_OPERATIONS_H

#include "data_structures.h"

Vertex calculate_centroid(Polyhedron *p);
void translate_polyhedron(Polyhedron *p, float dx, float dy, float dz);
void rotate_polyhedron_x(Polyhedron *p, float angle);
void rotate_polyhedron_y(Polyhedron *p, float angle);
void rotate_polyhedron_z(Polyhedron *p, float angle);
void slice_polyhedron(Polyhedron *p, float A, float B, float C, float D, Polyhedron **part1, Polyhedron **part2);

#endif
