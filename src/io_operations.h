#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H

#include "data_structures.h"

Polyhedron* read_polyhedron_from_file(const char *filename);
void write_polyhedron_to_file(Polyhedron *p, const char *filename);

#endif
