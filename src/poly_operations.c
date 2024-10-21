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
    
    // Temporary storage for vertices and edges
    Vertex *vertices_part1 = (Vertex*)malloc(max_new_vertices * sizeof(Vertex));
    Vertex *vertices_part2 = (Vertex*)malloc(max_new_vertices * sizeof(Vertex));
    
    Edge *edges_part1 = (Edge*)malloc(p->edge_count * sizeof(Edge));
    Edge *edges_part2 = (Edge*)malloc(p->edge_count * sizeof(Edge));
    
    // Track how edges and vertices are mapped
    int *vertex_mapping_part1 = (int*)malloc(p->vertex_count * sizeof(int));
    int *vertex_mapping_part2 = (int*)malloc(p->vertex_count * sizeof(int));
    
    int edge_count_part1 = 0;
    int edge_count_part2 = 0;

    // Process each vertex and classify it
    for (int i = 0; i < p->vertex_count; i++) {
        float distance = A * p->vertices[i].x + B * p->vertices[i].y + C * p->vertices[i].z + D;
        
        if (distance >= 0) {
            vertices_part1[vertex_count_part1] = p->vertices[i];
            vertex_mapping_part1[i] = vertex_count_part1++;
        } else {
            vertices_part2[vertex_count_part2] = p->vertices[i];
            vertex_mapping_part2[i] = vertex_count_part2++;
        }
    }

    // Now process edges that intersect the slicing plane
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
        
        // Classify edge based on its vertices
        if (dist_v1 >= 0 && dist_v2 >= 0) {
            edges_part1[edge_count_part1++] = (Edge){vertex_mapping_part1[v1], vertex_mapping_part1[v2]};
        } else if (dist_v1 < 0 && dist_v2 < 0) {
            edges_part2[edge_count_part2++] = (Edge){vertex_mapping_part2[v1], vertex_mapping_part2[v2]};
        }
    }

    // Allocate and populate part1 and part2
    if (vertex_count_part1 > 0) {
        *part1 = create_polyhedron(vertex_count_part1, edge_count_part1, p->face_count);  // Adjust as needed
        for (int i = 0; i < vertex_count_part1; i++) {
            (*part1)->vertices[i] = vertices_part1[i];
        }
        for (int i = 0; i < edge_count_part1; i++) {
            (*part1)->edges[i] = edges_part1[i];
        }
    }

    if (vertex_count_part2 > 0) {
        *part2 = create_polyhedron(vertex_count_part2, edge_count_part2, p->face_count);  // Adjust as needed
        for (int i = 0; i < vertex_count_part2; i++) {
            (*part2)->vertices[i] = vertices_part2[i];
        }
        for (int i = 0; i < edge_count_part2; i++) {
            (*part2)->edges[i] = edges_part2[i];
        }
    }

    // Free temporary storage
    free(vertices_part1);
    free(vertices_part2);
    free(edges_part1);
    free(edges_part2);
    free(vertex_mapping_part1);
    free(vertex_mapping_part2);
}


// Helper function to compute the volume of a tetrahedron given four points
float tetrahedron_volume(Vertex v0, Vertex v1, Vertex v2, Vertex v3) {
    float volume = (v1.x - v0.x) * ((v2.y - v0.y) * (v3.z - v0.z) - (v2.z - v0.z) * (v3.y - v0.y)) -
                   (v1.y - v0.y) * ((v2.x - v0.x) * (v3.z - v0.z) - (v2.z - v0.z) * (v3.x - v0.x)) +
                   (v1.z - v0.z) * ((v2.x - v0.x) * (v3.y - v0.y) - (v2.y - v0.y) * (v3.x - v0.x));
    return fabs(volume) / 6.0;
}

// Function to calculate the volume of a polyhedron using the tetrahedron method
float calculate_volume(Polyhedron *p) {
    Vertex origin = {0.0, 0.0, 0.0}; // Assume the origin is at (0, 0, 0)
    float total_volume = 0.0;

    for (int i = 0; i < p->face_count; i++) {
        Face face = p->faces[i];
        for (int j = 1; j < face.vertex_count - 1; j++) {
            Vertex v0 = p->vertices[face.vertices[0]];  // First vertex of the face
            Vertex v1 = p->vertices[face.vertices[j]];  // Current vertex
            Vertex v2 = p->vertices[face.vertices[j + 1]]; // Next vertex

            // Compute volume of tetrahedron formed by the face and the origin
            total_volume += tetrahedron_volume(origin, v0, v1, v2);
        }
    }
    return total_volume;
}

// Helper function to calculate the cross product of two vectors (for area computation)
Vertex cross_product(Vertex v1, Vertex v2) {
    Vertex result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

// Helper function to calculate the magnitude of a vector
float vector_magnitude(Vertex v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Helper function to calculate the area of a polygonal face using the "shoelace method"
float polygon_area(Polyhedron *p, Face face) {
    float area = 0.0;

    for (int i = 1; i < face.vertex_count - 1; i++) {
        Vertex v0 = p->vertices[face.vertices[0]];
        Vertex v1 = p->vertices[face.vertices[i]];
        Vertex v2 = p->vertices[face.vertices[i + 1]];

        // Calculate two vectors
        Vertex v1_minus_v0 = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
        Vertex v2_minus_v0 = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};

        // Calculate the cross product of these vectors (parallelogram area)
        Vertex cross = cross_product(v1_minus_v0, v2_minus_v0);

        // Add the magnitude of the cross product (half of it gives the area of a triangle)
        area += vector_magnitude(cross) / 2.0;
    }
    return area;
}

// Function to calculate the total surface area of the polyhedron
float calculate_surface_area(Polyhedron *p) {
    float total_area = 0.0;

    // Loop over each face and calculate the polygon's area
    for (int i = 0; i < p->face_count; i++) {
        total_area += polygon_area(p, p->faces[i]);
    }

    return total_area;
}
