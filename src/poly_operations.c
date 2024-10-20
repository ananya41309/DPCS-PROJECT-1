#include "poly_operations.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to calculate the centroid of the polyhedron
Vertex calculate_centroid(Polyhedron *p)
{
    Vertex centroid = {0.0, 0.0, 0.0};

    for (int i = 0; i < p->vertex_count; i++)
    {
        centroid.x += p->vertices[i].x;
        centroid.y += p->vertices[i].y;
        centroid.z += p->vertices[i].z;
    }

    centroid.x /= p->vertex_count;
    centroid.y /= p->vertex_count;
    centroid.z /= p->vertex_count;

    return centroid;
}

// Function to translate the polyhedron
void translate_polyhedron(Polyhedron *p, float dx, float dy, float dz)
{
    Vertex centroid = calculate_centroid(p);
    for (int i = 0; i < p->vertex_count; i++)
    {
        p->vertices[i].x += dx;
        p->vertices[i].y += dy;
        p->vertices[i].z += dz;
    }
    printf("Polyhedron translated by (%f, %f, %f)\n", dx, dy, dz);
}

// Function to rotate the polyhedron around the X-axis
void rotate_polyhedron_x(Polyhedron *p, float angle)
{
    Vertex centroid = calculate_centroid(p);
    translate_polyhedron(p, -centroid.x, -centroid.y, -centroid.z);

    float radians = angle * M_PI / 180.0;
    for (int i = 0; i < p->vertex_count; i++)
    {
        float y_new = p->vertices[i].y * cos(radians) - p->vertices[i].z * sin(radians);
        float z_new = p->vertices[i].y * sin(radians) + p->vertices[i].z * cos(radians);
        p->vertices[i].y = y_new;
        p->vertices[i].z = z_new;
    }

    translate_polyhedron(p, centroid.x, centroid.y, centroid.z);
    printf("Polyhedron rotated around X-axis by %f degrees\n", angle);
}

// Function to rotate the polyhedron around the Y-axis
void rotate_polyhedron_y(Polyhedron *p, float angle)
{
    Vertex centroid = calculate_centroid(p);
    translate_polyhedron(p, -centroid.x, -centroid.y, -centroid.z);

    float radians = angle * M_PI / 180.0;
    for (int i = 0; i < p->vertex_count; i++)
    {
        float x_new = p->vertices[i].x * cos(radians) + p->vertices[i].z * sin(radians);
        float z_new = -p->vertices[i].x * sin(radians) + p->vertices[i].z * cos(radians);
        p->vertices[i].x = x_new;
        p->vertices[i].z = z_new;
    }

    translate_polyhedron(p, centroid.x, centroid.y, centroid.z);
    printf("Polyhedron rotated around Y-axis by %f degrees\n", angle);
}

// Function to rotate the polyhedron around the Z-axis
void rotate_polyhedron_z(Polyhedron *p, float angle)
{
    Vertex centroid = calculate_centroid(p);
    translate_polyhedron(p, -centroid.x, -centroid.y, -centroid.z);

    float radians = angle * M_PI / 180.0;
    for (int i = 0; i < p->vertex_count; i++)
    {
        float x_new = p->vertices[i].x * cos(radians) - p->vertices[i].y * sin(radians);
        float y_new = p->vertices[i].x * sin(radians) + p->vertices[i].y * cos(radians);
        p->vertices[i].x = x_new;
        p->vertices[i].y = y_new;
    }

    translate_polyhedron(p, centroid.x, centroid.y, centroid.z);
    printf("Polyhedron rotated around Z-axis by %f degrees\n", angle);
}

// Function to evaluate the side of the plane for a vertex
float evaluate_plane(Vertex v, float A, float B, float C, float D)
{
    return A * v.x + B * v.y + C * v.z + D;
}

// Linear interpolation to find the intersection point between two vertices on an edge
Vertex interpolate_vertex(Vertex v1, Vertex v2, float t)
{
    Vertex v;
    v.x = v1.x + t * (v2.x - v1.x);
    v.y = v1.y + t * (v2.y - v1.y);
    v.z = v1.z + t * (v2.z - v1.z);
    return v;
}

// Function to slice the polyhedron
void slice_polyhedron(Polyhedron *p, float A, float B, float C, float D, Polyhedron **part1, Polyhedron **part2) {
    int vertex_count_part1 = 0;
    int vertex_count_part2 = 0;
    int max_new_vertices = p->vertex_count + p->edge_count; // max number of vertices after slicing
    
    // Temporary storage for vertices
    Vertex *vertices_part1 = (Vertex*)malloc(max_new_vertices * sizeof(Vertex));
    Vertex *vertices_part2 = (Vertex*)malloc(max_new_vertices * sizeof(Vertex));

    // Track which vertices go into part1 and part2
    int *vertex_mapping_part1 = (int*)malloc(p->vertex_count * sizeof(int));
    int *vertex_mapping_part2 = (int*)malloc(p->vertex_count * sizeof(int));

    // Process each edge and classify vertices
    for (int i = 0; i < p->vertex_count; i++) {
        float distance = A * p->vertices[i].x + B * p->vertices[i].y + C * p->vertices[i].z + D;

        if (distance >= 0) {
            // Vertex goes into part1
            vertices_part1[vertex_count_part1] = p->vertices[i];
            vertex_mapping_part1[i] = vertex_count_part1++;
        } else {
            // Vertex goes into part2
            vertices_part2[vertex_count_part2] = p->vertices[i];
            vertex_mapping_part2[i] = vertex_count_part2++;
        }
    }

    // Now process edges that intersect the plane
    for (int i = 0; i < p->edge_count; i++) {
        Edge edge = p->edges[i];
        int v1 = edge.v1;
        int v2 = edge.v2;
        
        float dist_v1 = A * p->vertices[v1].x + B * p->vertices[v1].y + C * p->vertices[v1].z + D;
        float dist_v2 = A * p->vertices[v2].x + B * p->vertices[v2].y + C * p->vertices[v2].z + D;

        if ((dist_v1 > 0 && dist_v2 < 0) || (dist_v1 < 0 && dist_v2 > 0)) {
            // Edge crosses the plane, calculate intersection point
            float t = dist_v1 / (dist_v1 - dist_v2);
            Vertex intersection_point;
            intersection_point.x = p->vertices[v1].x + t * (p->vertices[v2].x - p->vertices[v1].x);
            intersection_point.y = p->vertices[v1].y + t * (p->vertices[v2].y - p->vertices[v1].y);
            intersection_point.z = p->vertices[v1].z + t * (p->vertices[v2].z - p->vertices[v1].z);

            // Add intersection point to both parts
            vertices_part1[vertex_count_part1] = intersection_point;
            vertex_mapping_part1[p->vertex_count + i] = vertex_count_part1++;

            vertices_part2[vertex_count_part2] = intersection_point;
            vertex_mapping_part2[p->vertex_count + i] = vertex_count_part2++;
        }
    }

    // Allocate and populate part1 and part2 if they have vertices
    if (vertex_count_part1 > 0) {
        *part1 = create_polyhedron(vertex_count_part1, p->edge_count, p->face_count);  // Adjust as needed
        for (int i = 0; i < vertex_count_part1; i++) {
            (*part1)->vertices[i] = vertices_part1[i];
        }
    }

    if (vertex_count_part2 > 0) {
        *part2 = create_polyhedron(vertex_count_part2, p->edge_count, p->face_count);  // Adjust as needed
        for (int i = 0; i < vertex_count_part2; i++) {
            (*part2)->vertices[i] = vertices_part2[i];
        }
    }

    // Free temporary storage
    free(vertices_part1);
    free(vertices_part2);
    free(vertex_mapping_part1);
    free(vertex_mapping_part2);
}
