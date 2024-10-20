#include "visualization.h"
#include <stdio.h>
#include <SDL2/SDL.h>

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
