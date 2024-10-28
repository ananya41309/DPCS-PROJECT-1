#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "data_structures.h"

void visualize_polyhedron(Polyhedron *p);
void visualize_orthographic_projection(Vertex *projected_vertices, Edge *edges, int vertex_count, int edge_count, const char *view_name);

#endif
