#include "io_operations.h"
#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create a polyhedron
Polyhedron *create_polyhedron(int vertex_count, int edge_count, int face_count)
{
    Polyhedron *p = (Polyhedron *)malloc(sizeof(Polyhedron));
    p->vertices = (Vertex *)malloc(vertex_count * sizeof(Vertex));
    p->edges = (Edge *)malloc(edge_count * sizeof(Edge));
    p->faces = (Face *)malloc(face_count * sizeof(Face));
    p->vertex_count = vertex_count;
    p->edge_count = edge_count;
    p->face_count = face_count;
    return p;
}

// Free the allocated memory
void free_polyhedron(Polyhedron *p)
{
    free(p->vertices);
    free(p->edges);
    for (int i = 0; i < p->face_count; i++)
    {
        free(p->faces[i].vertices);
    }
    free(p->faces);
    free(p);
}


// Function to read polyhedron data from a file
Polyhedron *read_polyhedron_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    int vertex_count, edge_count, face_count;
    fscanf(file, "Vertex Count: %d\n", &vertex_count);
    fscanf(file, "Edge Count: %d\n", &edge_count);
    fscanf(file, "Face Count: %d\n", &face_count);

    Polyhedron *p = create_polyhedron(vertex_count, edge_count, face_count);

    // Read vertices
    for (int i = 0; i < vertex_count; i++)
    {
        fscanf(file, "%f %f %f\n", &p->vertices[i].x, &p->vertices[i].y, &p->vertices[i].z);
    }

    // Read edges
    for (int i = 0; i < edge_count; i++)
    {
        fscanf(file, "%d %d\n", &p->edges[i].v1, &p->edges[i].v2);
    }

    // Read faces
    for (int i = 0; i < face_count; i++)
    {
        fscanf(file, "%d", &p->faces[i].vertex_count);
        p->faces[i].vertices = (int *)malloc(p->faces[i].vertex_count * sizeof(int));
        for (int j = 0; j < p->faces[i].vertex_count; j++)
        {
            fscanf(file, "%d", &p->faces[i].vertices[j]);
        }
    }

    fclose(file);
    return p;
}

// Function to write polyhedron data to a file
void write_polyhedron_to_file(Polyhedron *p, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    fprintf(file, "Vertex Count: %d\n", p->vertex_count);
    fprintf(file, "Edge Count: %d\n", p->edge_count);
    fprintf(file, "Face Count: %d\n", p->face_count);

    // Write vertices
    for (int i = 0; i < p->vertex_count; i++)
    {
        fprintf(file, "%f %f %f\n", p->vertices[i].x, p->vertices[i].y, p->vertices[i].z);
    }

    // Write edges
    for (int i = 0; i < p->edge_count; i++)
    {
        fprintf(file, "%d %d\n", p->edges[i].v1, p->edges[i].v2);
    }

    // Write faces
    for (int i = 0; i < p->face_count; i++)
    {
        fprintf(file, "%d ", p->faces[i].vertex_count);
        for (int j = 0; j < p->faces[i].vertex_count; j++)
        {
            fprintf(file, "%d ", p->faces[i].vertices[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
