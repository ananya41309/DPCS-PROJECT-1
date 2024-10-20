#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINE_LENGTH 100

// Data Structures
typedef struct
{
    float x, y, z;
} Vertex;

typedef struct
{
    int v1, v2;
} Edge;

typedef struct
{
    int *vertices;
    int vertex_count;
} Face;

typedef struct
{
    Vertex *vertices;
    int vertex_count;

    Edge *edges;
    int edge_count;

    Face *faces;
    int face_count;
} Polyhedron;

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

// visualize
void visualize_polyhedron(Polyhedron *p)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Initialization Failed: %s\n", SDL_GetError());
        return;
    }

    SDL_Window *window = SDL_CreateWindow("Polyhedron Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Parameters for perspective projection
    float focal_length = 500.0f; // You can tweak this value
    float distance = 5.0f;       // Distance from the camera

    // Render edges with perspective projection
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < p->edge_count; i++)
    {
        int v1 = p->edges[i].v1;
        int v2 = p->edges[i].v2;

        // Apply perspective projection to vertex v1
        float x1 = (focal_length * p->vertices[v1].x) / (p->vertices[v1].z + distance);
        float y1 = (focal_length * p->vertices[v1].y) / (p->vertices[v1].z + distance);

        // Apply perspective projection to vertex v2
        float x2 = (focal_length * p->vertices[v2].x) / (p->vertices[v2].z + distance);
        float y2 = (focal_length * p->vertices[v2].y) / (p->vertices[v2].z + distance);

        // Convert to screen coordinates (centering the polyhedron)
        int screen_x1 = (int)(x1 + 320); // 320 is half of the window width (640 / 2)
        int screen_y1 = (int)(y1 + 240); // 240 is half of the window height (480 / 2)

        int screen_x2 = (int)(x2 + 320);
        int screen_y2 = (int)(y2 + 240);

        // Draw the edge
        SDL_RenderDrawLine(renderer, screen_x1, screen_y1, screen_x2, screen_y2);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000); // Display for 5 seconds

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#include <stdbool.h>

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
void slice_polyhedron(Polyhedron *p, float A, float B, float C, float D, Polyhedron **part1, Polyhedron **part2)
{
    int i;
    // Step 1: Classify vertices with respect to the plane
    bool *above_plane = (bool *)malloc(p->vertex_count * sizeof(bool));
    for (i = 0; i < p->vertex_count; i++)
    {
        above_plane[i] = evaluate_plane(p->vertices[i], A, B, C, D) >= 0.0;
    }

    // Step 2: Process edges to find intersections with the plane
    Vertex *new_vertices = (Vertex *)malloc(p->edge_count * sizeof(Vertex)); // To store new intersection vertices
    int new_vertex_count = 0;

    for (i = 0; i < p->edge_count; i++)
    {
        int v1_index = p->edges[i].v1;
        int v2_index = p->edges[i].v2;

        bool v1_above = above_plane[v1_index];
        bool v2_above = above_plane[v2_index];

        if (v1_above != v2_above)
        {
            // Edge crosses the plane, so compute intersection
            Vertex v1 = p->vertices[v1_index];
            Vertex v2 = p->vertices[v2_index];

            // Find the intersection point using parameter t
            float t = -evaluate_plane(v1, A, B, C, D) / (evaluate_plane(v2, A, B, C, D) - evaluate_plane(v1, A, B, C, D));
            Vertex intersection = interpolate_vertex(v1, v2, t);

            new_vertices[new_vertex_count++] = intersection; // Store the intersection vertex
        }
    }

    // TODO: Use the classified vertices, edges, and new intersection vertices to construct new faces and
    //       create the two parts (part1 and part2).
    //       You will need to update the edge and face data structures of each part to include the correct vertices.

    // Free the above_plane array once done
    free(above_plane);
    free(new_vertices);
}

// Main function
int main(int argc, char *argv[])
{
    float A, B, C, D;
    // Ask user for the input file
    char input_filename[MAX_LINE_LENGTH];
    printf("Enter the input polyhedron data file: ");
    scanf("%s", input_filename);

    // Read the polyhedron from file
    Polyhedron *polyhedron = read_polyhedron_from_file(input_filename);
    if (!polyhedron)
    {
        return 1; // Exit if the polyhedron couldn't be read
    }

    // Visualize the loaded polyhedron
    printf("Visualizing the polyhedron loaded from %s\n", input_filename);
    visualize_polyhedron(polyhedron);

    char operation_choice;
    while (1)
    {
        // Ask user what operation to perform: rotate, translate, or exit
        printf("\nChoose operation: (r)otate, (t)ranslate, (s)lice, (e)xit: ");
        scanf(" %c", &operation_choice);

        if (operation_choice == 't')
        {
            // Perform translation
            float dx, dy, dz;
            printf("Enter translation values (dx dy dz): ");
            scanf("%f %f %f", &dx, &dy, &dz);
            translate_polyhedron(polyhedron, dx, dy, dz);

            // Save the translated polyhedron to an output file
            char translated_filename[MAX_LINE_LENGTH];
            snprintf(translated_filename, sizeof(translated_filename), "%s_translated_object.txt", input_filename);
            write_polyhedron_to_file(polyhedron, translated_filename);
            printf("Translated polyhedron saved to %s\n", translated_filename);

            // Visualize the translated polyhedron
            visualize_polyhedron(polyhedron);
        }
        else if (operation_choice == 'r')
        {
            // Perform rotation
            char axis_choice;
            float angle;
            printf("Enter axis for rotation (x, y, z): ");
            scanf(" %c", &axis_choice);

            // Validate axis input
            if (axis_choice != 'x' && axis_choice != 'y' && axis_choice != 'z')
            {
                printf("Invalid axis! Please enter 'x', 'y', or 'z'.\n");
                continue;
            }

            printf("Enter rotation angle (degrees): ");
            scanf("%f", &angle);

            // Call the appropriate rotation function based on the axis
            if (axis_choice == 'x')
            {
                rotate_polyhedron_x(polyhedron, angle);
            }
            else if (axis_choice == 'y')
            {
                rotate_polyhedron_y(polyhedron, angle);
            }
            else if (axis_choice == 'z')
            {
                rotate_polyhedron_z(polyhedron, angle);
            }

            // Save the rotated polyhedron to an output file
            char rotated_filename[MAX_LINE_LENGTH];
            snprintf(rotated_filename, sizeof(rotated_filename), "%s_rotated_%c_object.txt", input_filename, axis_choice);
            write_polyhedron_to_file(polyhedron, rotated_filename);
            printf("Rotated polyhedron saved to %s\n", rotated_filename);

            // Visualize the rotated polyhedron
            visualize_polyhedron(polyhedron);
        }
        else if (operation_choice == 's')
        {
            printf("Enter the normal vector of the slicing plane (A B C): ");
            scanf("%f %f %f", &A, &B, &C);
            printf("Enter the distance from the origin (D): ");
            scanf("%f", &D);

            // Variables to hold the two new parts
            Polyhedron *part1 = NULL, *part2 = NULL;

            // Call the slice_polyhedron function
            slice_polyhedron(polyhedron, A, B, C, D, &part1, &part2);

            // Write the two new parts to files
            if (part1 != NULL)
            {
                write_polyhedron_to_file(part1, "part1_sliced.txt");
                visualize_polyhedron(part1);
            }
            if (part2 != NULL)
            {
                write_polyhedron_to_file(part2, "part2_sliced.txt");
                visualize_polyhedron(part2);
            }

            // Free memory for the new parts
            if (part1)
                free_polyhedron(part1);
            if (part2)
                free_polyhedron(part2);
        }
        else if (operation_choice == 'e')
        {
            // Exit the program
            printf("Exiting program...\n");
            break;
        }
        else
        {
            // Invalid input, remind user
            printf("Invalid choice! Please enter 'r', 't', or 'e'.\n");
        }
    }

    // Free allocated memory before exiting
    free_polyhedron(polyhedron);

    return 0;
}

