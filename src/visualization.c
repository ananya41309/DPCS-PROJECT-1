#include "visualization.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

// visualize
void visualize_polyhedron(Polyhedron *p)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Initialization Failed: %s\n", SDL_GetError());
        return;
    }

    // Create an SDL window
    SDL_Window *window = SDL_CreateWindow("Polyhedron Visualization", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          640, 480, 
                                          SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Create a renderer with hardware acceleration and vsync
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Main loop flag
    bool quit = false;
    SDL_Event e;

    // Parameters for perspective projection
    float focal_length = 500.0f; 
    float distance = 5.0f;       // Distance from the camera

    // Main loop to keep the window open
    while (!quit)
    {
        // Event handler
        while (SDL_PollEvent(&e) != 0)
        {
            // Quit if the window is closed
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Set the background color (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render edges with perspective projection
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black for the edges
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
            int screen_x1 = (int)(x1 + 320); 
            int screen_y1 = (int)(y1 + 240); 

            int screen_x2 = (int)(x2 + 320);
            int screen_y2 = (int)(y2 + 240);

            // Draw the edge
            SDL_RenderDrawLine(renderer, screen_x1, screen_y1, screen_x2, screen_y2);
        }

        // Update the screen
        SDL_RenderPresent(renderer);

        
        SDL_Delay(16); 
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Function to create a 2D SDL visualization of the projected polyhedron
void visualize_orthographic_projection(Vertex *projected_vertices, Edge *edges, int vertex_count, int edge_count, const char *view_name) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization Failed: %s\n", SDL_GetError());
        return;
    }

    SDL_Window *window = SDL_CreateWindow(view_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Create renderer with hardware acceleration and vsync enabled
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Set background color to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Set edge color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < edge_count; i++) {
            int v1 = edges[i].v1;
            int v2 = edges[i].v2;
            SDL_RenderDrawLine(renderer,
                               (int)(projected_vertices[v1].x * 100 + 320), (int)(projected_vertices[v1].y * 100 + 240),
                               (int)(projected_vertices[v2].x * 100 + 320), (int)(projected_vertices[v2].y * 100 + 240));
        }

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
