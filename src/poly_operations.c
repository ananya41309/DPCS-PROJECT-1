#include "poly_operations.h"
#include "visualization.h"
#include "data_structures.h"
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

// Project the polyhedron onto the YZ-plane (Front view)
void project_front_view(Polyhedron *p) {
    Vertex *projected_vertices = (Vertex *)malloc(p->vertex_count * sizeof(Vertex));
    for (int i = 0; i < p->vertex_count; i++) {
        projected_vertices[i].x = p->vertices[i].y;  // YZ-plane: Use y as x-coordinate
        projected_vertices[i].y = p->vertices[i].z;  // Use z as y-coordinate
    }
    visualize_orthographic_projection(projected_vertices, p->edges, p->vertex_count, p->edge_count, "Front View (YZ-plane)");
    free(projected_vertices);
}

// Project the polyhedron onto the XZ-plane (Top view)
void project_top_view(Polyhedron *p) {
    Vertex *projected_vertices = (Vertex *)malloc(p->vertex_count * sizeof(Vertex));
    for (int i = 0; i < p->vertex_count; i++) {
        projected_vertices[i].x = p->vertices[i].x;  // XZ-plane: Use x as x-coordinate
        projected_vertices[i].y = p->vertices[i].z;  // Use z as y-coordinate
    }
    visualize_orthographic_projection(projected_vertices, p->edges, p->vertex_count, p->edge_count, "Top View (XZ-plane)");
    free(projected_vertices);
}

// Project the polyhedron onto the XY-plane (Right view)
void project_side_view(Polyhedron *p) {
    Vertex *projected_vertices = (Vertex *)malloc(p->vertex_count * sizeof(Vertex));
    for (int i = 0; i < p->vertex_count; i++) {
        projected_vertices[i].x = p->vertices[i].x;  // XY-plane: Use x as x-coordinate
        projected_vertices[i].y = p->vertices[i].y;  // Use y as y-coordinate
    }
    visualize_orthographic_projection(projected_vertices, p->edges, p->vertex_count, p->edge_count, "Side View (XY-plane)");
    free(projected_vertices);
}

// Helper function to match vertices based on proximity (to account for minor numerical differences)
int find_matching_vertex(float x, float y, float z, ReconstructedVertex *vertices, int count) {
    for (int i = 0; i < count; i++) {
        if (!vertices[i].matched &&
            fabs(vertices[i].x - x) < 1e-3 &&
            fabs(vertices[i].y - y) < 1e-3 &&
            fabs(vertices[i].z - z) < 1e-3) {
            vertices[i].matched = 1; // Mark as matched
            return i;
        }
    }
    return -1; // No match found
}

// Extract vertices from each view projection (dummy function for demonstration)
void extract_vertices_from_projection(Vertex *vertices, int vertex_count, char view) {
    for (int i = 0; i < vertex_count; i++) {
        switch (view) {
            case 'f':  // Front view (YZ-plane)
                vertices[i].x = 0.0; // x is unknown in front view
                break;
            case 't':  // Top view (XZ-plane)
                vertices[i].y = 0.0; // y is unknown in top view
                break;
            case 's':  // Side view (XY-plane)
                vertices[i].z = 0.0; // z is unknown in side view
                break;
        }
    }
}

// Find or create a matching vertex in the reconstructed vertex list
int find_or_create_vertex(Vertex v, Vertex *reconstructed_vertices, int *reconstructed_count, int max_vertices) {
    for (int i = 0; i < *reconstructed_count; i++) {
        if (fabs(reconstructed_vertices[i].x - v.x) < 1e-3 &&
            fabs(reconstructed_vertices[i].y - v.y) < 1e-3 &&
            fabs(reconstructed_vertices[i].z - v.z) < 1e-3) {
            return i; // Vertex found
        }
    }
    // If not found, add the new vertex if within bounds
    if (*reconstructed_count < max_vertices) {
        reconstructed_vertices[*reconstructed_count] = v;
        (*reconstructed_count)++;
        return *reconstructed_count - 1; // Index of new vertex
    } else {
        fprintf(stderr, "Error: Vertex count exceeds maximum allowed (%d).\n", max_vertices);
        exit(1);
    }
}

// Reconstruct the polyhedron with vertices, edges, and faces from 2D projections
Polyhedron* reconstruct_polyhedron_from_views(Vertex *front_view, int front_count,
                                              Vertex *top_view, int top_count,
                                              Vertex *side_view, int side_count) {
    int max_vertices = front_count + top_count + side_count; // Estimate max vertices
    Vertex *reconstructed_vertices = (Vertex *)malloc(max_vertices * sizeof(Vertex));
    int reconstructed_count = 0;

    if (!reconstructed_vertices) {
        fprintf(stderr, "Error: Memory allocation failed for vertices.\n");
        exit(1);
    }

    // Populate vertices from Front View (YZ-plane)
    for (int i = 0; i < front_count; i++) {
        Vertex v = {0.0, front_view[i].x, front_view[i].y}; // x is 0 in front view
        find_or_create_vertex(v, reconstructed_vertices, &reconstructed_count, max_vertices);
    }

    // Populate vertices from Top View (XZ-plane)
    for (int i = 0; i < top_count; i++) {
        Vertex v = {top_view[i].x, 0.0, top_view[i].y}; // y is 0 in top view
        find_or_create_vertex(v, reconstructed_vertices, &reconstructed_count, max_vertices);
    }

    // Populate vertices from Side View (XY-plane)
    for (int i = 0; i < side_count; i++) {
        Vertex v = {side_view[i].x, side_view[i].y, 0.0}; // z is 0 in side view
        find_or_create_vertex(v, reconstructed_vertices, &reconstructed_count, max_vertices);
    }

    // Allocate memory for the Polyhedron and copy vertices
    Polyhedron *polyhedron = (Polyhedron *)malloc(sizeof(Polyhedron));
    if (!polyhedron) {
        fprintf(stderr, "Error: Memory allocation failed for polyhedron.\n");
        free(reconstructed_vertices);
        exit(1);
    }
    
    polyhedron->vertices = (Vertex *)malloc(reconstructed_count * sizeof(Vertex));
    if (!polyhedron->vertices) {
        fprintf(stderr, "Error: Memory allocation failed for polyhedron vertices.\n");
        free(reconstructed_vertices);
        free(polyhedron);
        exit(1);
    }
    
    polyhedron->vertex_count = reconstructed_count;
    for (int i = 0; i < reconstructed_count; i++) {
        polyhedron->vertices[i] = reconstructed_vertices[i];
    }

    // Define edges based on adjacency (simplified assumption)
    int max_edges = reconstructed_count * 3;
    polyhedron->edges = (Edge *)malloc(max_edges * sizeof(Edge));
    if (!polyhedron->edges) {
        fprintf(stderr, "Error: Memory allocation failed for polyhedron edges.\n");
        free(reconstructed_vertices);
        free(polyhedron->vertices);
        free(polyhedron);
        exit(1);
    }
    polyhedron->edge_count = 0;

    // Sample adjacency-based edge definition (adjust based on actual view)
    for (int i = 0; i < reconstructed_count - 1; i++) {
        polyhedron->edges[polyhedron->edge_count].v1 = i;
        polyhedron->edges[polyhedron->edge_count].v2 = i + 1;
        polyhedron->edge_count++;
    }

    // Define simple quad faces (adjust according to actual needs)
    polyhedron->faces = (Face *)malloc((reconstructed_count / 4) * sizeof(Face));
    if (!polyhedron->faces) {
        fprintf(stderr, "Error: Memory allocation failed for polyhedron faces.\n");
        free(reconstructed_vertices);
        free(polyhedron->vertices);
        free(polyhedron->edges);
        free(polyhedron);
        exit(1);
    }
    polyhedron->face_count = 0;

    for (int i = 0; i < reconstructed_count; i += 4) {
        polyhedron->faces[polyhedron->face_count].vertices = (int *)malloc(4 * sizeof(int));
        if (!polyhedron->faces[polyhedron->face_count].vertices) {
            fprintf(stderr, "Error: Memory allocation failed for face vertices.\n");
            free(reconstructed_vertices);
            free(polyhedron->vertices);
            free(polyhedron->edges);
            free(polyhedron);
            exit(1);
        }

        polyhedron->faces[polyhedron->face_count].vertex_count = 4;
        for (int j = 0; j < 4; j++) {
            polyhedron->faces[polyhedron->face_count].vertices[j] = i + j;
        }
        polyhedron->face_count++;
    }

    // Free the temporary reconstructed vertices array
    free(reconstructed_vertices);

    return polyhedron;
}
