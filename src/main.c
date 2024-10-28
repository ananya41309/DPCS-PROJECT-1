#include <stdio.h>
#include<stdlib.h>
#include "data_structures.h"
#include "io_operations.h"
#include "poly_operations.h"
#include "visualization.h"

#define MAX_LINE_LENGTH 100

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
    // Calculate the volume
    float volume = calculate_volume(polyhedron);
    printf("Volume of the polyhedron: %f\n", volume);

    // Calculate the surface area
    float surface_area = calculate_surface_area(polyhedron);
    printf("Surface area of the polyhedron: %f\n", surface_area);
    // Visualize the loaded polyhedron
    printf("Visualizing the polyhedron loaded from %s\n", input_filename);
    visualize_polyhedron(polyhedron);

    char view_choice = 'a';
    while (view_choice != 'e')
    {
        printf("Choose an orthographic view (t: Top, f: Front, s: Side, e: Exit): ");
    scanf(" %c", &view_choice);
        switch (view_choice)
        {
        case 't':
            project_top_view(polyhedron);
            break;
        case 'f':
            project_front_view(polyhedron);
            break;
        case 's':
            project_side_view(polyhedron);
            break;
        case 'e':
            break;
        default:
            printf("Invalid choice. Please enter 't', 'f', 's' or 'e'.\n");
            break;
        }
    }

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
            // Calculate the volume
            float volume = calculate_volume(polyhedron);
            printf("Volume of the polyhedron: %f\n", volume);

            // Calculate the surface area
            float surface_area = calculate_surface_area(polyhedron);
            printf("Surface area of the polyhedron: %f\n", surface_area);
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
            // Calculate the volume
            float volume = calculate_volume(polyhedron);
            printf("Volume of the polyhedron: %f\n", volume);

            // Calculate the surface area
            float surface_area = calculate_surface_area(polyhedron);
            printf("Surface area of the polyhedron: %f\n", surface_area);
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
                // Calculate the volume
                float volume = calculate_volume(polyhedron);
                printf("Volume of the polyhedron: %f\n", volume);

                // Calculate the surface area
                float surface_area = calculate_surface_area(polyhedron);
                printf("Surface area of the polyhedron: %f\n", surface_area);
                visualize_polyhedron(part1);
            }
            if (part2 != NULL)
            {
                write_polyhedron_to_file(part2, "part2_sliced.txt");
                // Calculate the volume
                float volume = calculate_volume(polyhedron);
                printf("Volume of the polyhedron: %f\n", volume);

                // Calculate the surface area
                float surface_area = calculate_surface_area(polyhedron);
                printf("Surface area of the polyhedron: %f\n", surface_area);
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

     // Read vertices from each file
    Vertex *front_view = NULL;
    Vertex *top_view = NULL;
    Vertex *side_view = NULL;

    int front_count = read_vertices_from_file("front_view.txt", &front_view, 'f');
    int top_count = read_vertices_from_file("top_view.txt", &top_view, 't');
    int side_count = read_vertices_from_file("side_view.txt", &side_view, 's');

    if (front_count == 0 || top_count == 0 || side_count == 0) {
        fprintf(stderr, "Error: Failed to read vertices from one or more files.\n");
        free(front_view);
        free(top_view);
        free(side_view);
        return 1;
    }
 printf("Reconstructing Polyhedron:\n");
    // Reconstruct the polyhedron from the orthographic projections
    Polyhedron *reconstructed_polyhedron = reconstruct_polyhedron_from_views(
        front_view, front_count, top_view, top_count, side_view, side_count
    );

    // Output the reconstructed vertices for verification
    printf("Reconstructed Polyhedron Vertices:\n");
    for (int i = 0; i < reconstructed_polyhedron->vertex_count; i++) {
        printf("Vertex %d: (%.2f, %.2f, %.2f)\n", i + 1,
               reconstructed_polyhedron->vertices[i].x,
               reconstructed_polyhedron->vertices[i].y,
               reconstructed_polyhedron->vertices[i].z);
    }

    // Visualize the reconstructed polyhedron (assuming SDL is set up)
    write_polyhedron_to_file(reconstructed_polyhedron, "reconstructed_polyhedron.txt");
    visualize_polyhedron(reconstructed_polyhedron);

    // Free allocated memory
    free(front_view);
    free(top_view);
    free(side_view);
    free_polyhedron(reconstructed_polyhedron);


    return 0;
}