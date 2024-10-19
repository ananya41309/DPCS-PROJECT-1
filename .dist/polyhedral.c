#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Data Structure
// Define a 3D vector (vertex)
typedef struct {
    float x, y, z;
} Vertex;

// Define an Edge between two vertices
typedef struct {
    int v1, v2;  // Indices of vertices
} Edge;

// Define a Face (Polygon with n vertices)
typedef struct {
    int *vertices;  // Array of vertex indices
    int vertex_count;
} Face;

// Define a Polyhedron consisting of vertices, edges, and faces
typedef struct {
    Vertex *vertices;
    int vertex_count;
    
    Edge *edges;
    int edge_count;
    
    Face *faces;
    int face_count;
} Polyhedron;

// Function to create a polyhedron
Polyhedron* create_polyhedron(int vertex_count, int edge_count, int face_count) {
    Polyhedron *p = (Polyhedron*)malloc(sizeof(Polyhedron));
    p->vertices = (Vertex*)malloc(vertex_count * sizeof(Vertex));
    p->edges = (Edge*)malloc(edge_count * sizeof(Edge));
    p->faces = (Face*)malloc(face_count * sizeof(Face));
    p->vertex_count = vertex_count;
    p->edge_count = edge_count;
    p->face_count = face_count;
    return p;
}

// Free the allocated memory
void free_polyhedron(Polyhedron *p) {
    free(p->vertices);
    free(p->edges);
    for (int i = 0; i < p->face_count; i++) {
        free(p->faces[i].vertices);
    }
    free(p->faces);
    free(p);
}

//input

void input_vertices(Polyhedron *p) {
    printf("Enter the coordinates of %d vertices (x y z):\n", p->vertex_count);
    for (int i = 0; i < p->vertex_count; i++) {
        printf("Vertex %d: ", i + 1);
        scanf("%f %f %f", &p->vertices[i].x, &p->vertices[i].y, &p->vertices[i].z);
    }
}

void input_edges(Polyhedron *p) {
    printf("Enter the %d edges (vertex1_index vertex2_index):\n", p->edge_count);
    for (int i = 0; i < p->edge_count; i++) {
        printf("Edge %d: ", i + 1);
        scanf("%d %d", &p->edges[i].v1, &p->edges[i].v2);
    }
}

void input_faces(Polyhedron *p) {
    for (int i = 0; i < p->face_count; i++) {
        printf("Enter the number of vertices for face %d: ", i + 1);
        scanf("%d", &p->faces[i].vertex_count);
        p->faces[i].vertices = (int*)malloc(p->faces[i].vertex_count * sizeof(int));
        printf("Enter the vertices for face %d: ", i + 1);
        for (int j = 0; j < p->faces[i].vertex_count; j++) {
            scanf("%d", &p->faces[i].vertices[j]);
        }
    }
}

//translate
void translate(Vertex vertices[], int num_vertices, float tx, float ty, float tz) {
    for (int i = 0; i < num_vertices; i++) {
        // Apply translation
        vertices[i].x += tx;
        vertices[i].y += ty;
        vertices[i].z += tz;
    }
}

void translate_polyhedron(Polyhedron *p, float dx, float dy, float dz) {
    for (int i = 0; i < p->vertex_count; i++) {
        p->vertices[i].x += dx;
        p->vertices[i].y += dy;
        p->vertices[i].z += dz;
    }
    printf("Polyhedron translated by (%f, %f, %f)\n", dx, dy, dz);
}

//rotation

void rotate_polyhedron_z(Polyhedron *p, float angle) {
    float radians = angle * M_PI / 180.0;
    for (int i = 0; i < p->vertex_count; i++) {
        float x_new = p->vertices[i].x * cos(radians) - p->vertices[i].y * sin(radians);
        float y_new = p->vertices[i].x * sin(radians) + p->vertices[i].y * cos(radians);
        p->vertices[i].x = x_new;
        p->vertices[i].y = y_new;
    }
    printf("Polyhedron rotated around Z-axis by %f degrees\n", angle);
}
//visualize
void visualize_polyhedron(Polyhedron *p) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization Failed: %s\n", SDL_GetError());
        return;
    }

    SDL_Window *window = SDL_CreateWindow("Polyhedron Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render edges (simple example for 2D projection)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < p->edge_count; i++) {
        int v1 = p->edges[i].v1;
        int v2 = p->edges[i].v2;
        SDL_RenderDrawLine(renderer, 
                           (int)p->vertices[v1].x * 100 + 320, (int)p->vertices[v1].y * 100 + 240,
                           (int)p->vertices[v2].x * 100 + 320, (int)p->vertices[v2].y * 100 + 240);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000); // Display for 5 seconds

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



// Declare all functions here (from the previous code)

int main(int argc, char* argv[]) {
    int vertex_count, edge_count, face_count;
    
    // Ask the user for the number of vertices, edges, and faces
    printf("Enter the number of vertices: ");
    scanf("%d", &vertex_count);
    printf("Enter the number of edges: ");
    scanf("%d", &edge_count);
    printf("Enter the number of faces: ");
    scanf("%d", &face_count);
    
    // Create the polyhedron with user-specified vertices, edges, and faces
    Polyhedron *polyhedron = create_polyhedron(vertex_count, edge_count, face_count);
    
    // Input the vertices, edges, and faces
    input_vertices(polyhedron);
    input_edges(polyhedron);
    input_faces(polyhedron);

     // Visualize the polyhedron using SDL
    char visualize_choice;
    printf("Do you want to visualize the polyhedron (y/n)? ");
    scanf(" %c", &visualize_choice);
    if (visualize_choice == 'y') {
        visualize_polyhedron(polyhedron);
    }
    
    // Perform translation
    /*float dx, dy, dz;
    printf("Enter translation values (dx dy dz): ");
    scanf("%f %f %f", &dx, &dy, &dz);
    translate_polyhedron(polyhedron, dx, dy, dz);

     // Visualize the polyhedron using SDL
    
    printf("Do you want to visualize the polyhedron (y/n)? ");
    scanf(" %c", &visualize_choice);
    if (visualize_choice == 'y') {
        visualize_polyhedron(polyhedron);
    }*/
    
    // Perform rotation around Z-axis
    float angle;
    printf("Enter rotation angle around Z-axis (degrees): ");
    scanf("%f", &angle);
    rotate_polyhedron_z(polyhedron, angle);
    
    // Visualize the polyhedron using SDL
    
    printf("Do you want to visualize the polyhedron (y/n)? ");
    scanf(" %c", &visualize_choice);
    if (visualize_choice == 'y') {
        visualize_polyhedron(polyhedron);
    }

    // Free the allocated memory
    free_polyhedron(polyhedron);
    
    return 0;
}



