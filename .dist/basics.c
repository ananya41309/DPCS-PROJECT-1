#include <stdio.h>
#include <math.h>

// Define a structure for a point in 2D space
typedef struct {
    float x;
    float y;
} Point;

// Define a structure for a line (with two points)
typedef struct {
    Point start;
    Point end;
} Line;

// Define a structure for a circle
typedef struct {
    Point center;
    float radius;
} Circle;

// Function to read a point from the user
void readPoint(Point *p) {
    printf("Enter x and y coordinates: ");
    scanf("%f %f", &p->x, &p->y);
}

// Function to print a point
void printPoint(Point p) {
    printf("Point: (%.2f, %.2f)\n", p.x, p.y);
}

// Function to translate a point
void translatePoint(Point *p, float dx, float dy) {
    p->x += dx;
    p->y += dy;
}

int main() {
    Point p;
    readPoint(&p);        // Input point
    printPoint(p);        // Output original point

    translatePoint(&p, 10, 5);  // Translate by (10, 5)
    printPoint(p);        // Output translated point
    
    return 0;
}
