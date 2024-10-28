#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H

#include "data_structures.h"

Polyhedron* read_polyhedron_from_file(const char *filename);
void write_polyhedron_to_file(Polyhedron *p, const char *filename);
Polyhedron *create_polyhedron(int vertex_count, int edge_count, int face_count);
void free_polyhedron(Polyhedron *p);
int read_vertices_from_file(const char *filename, Vertex **vertices, char view);

#endif
