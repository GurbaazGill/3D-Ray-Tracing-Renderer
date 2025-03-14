/***********************************************************/
/*      GURBAAZ GILL                                       */
/*      SPHERES.C FOR MILESTONE 1                          */
/***********************************************************/

#include "spheres.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//////////////////////////////////////////////////// Initialize the World structure ////////////////////////////////////////////////////
void worldInit(World *world) {
    
    // Start with size 0 and capacity 1
    world->size = 0;
    world->capacity = 1;  
    world->spheres = malloc(world->capacity * sizeof(Sphere *));

    if (!world->spheres) {
        printf("Memory Allocation Error: Failed to initialize sphere array");
        exit(1);
    }
}



//////////////////////////////////////////////////// Free memory allocated for the World structure ////////////////////////////////////////////////////
void freeWorld(World *world) {
    // Free each Sphere
    for (int i = 0; i < world->size; i++) {
        free(world->spheres[i]);  
    }

     // Free the array itself
    free(world->spheres);  

    world->spheres = NULL;
    world->size = world->capacity = 0;
}



//////////////////////////////////////////////////// Add a Sphere to the World structure ////////////////////////////////////////////////////
void addSphere(World *world, Sphere *sphere) {

    // if increasing capacity needed
    if (world->size >= world->capacity) {
        // Increase capacity by doubling it
        world->capacity *= 2;
        Sphere **newSpheres = realloc(world->spheres, world->capacity * sizeof(Sphere *));
        // Error in case memory alocation fails
        if (!newSpheres) {
            printf("Memory Allocation Error: Failed to allocate new sphere");
            freeWorld(world); 
            exit(1);
        }
        world->spheres = newSpheres;
    } 
    // Add the new sphere
    world->spheres[world->size] = sphere; 
    world->size++;
}



//////////////////////////////////////////////////// Create a new Sphere ////////////////////////////////////////////////////
Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *sphere = malloc(sizeof(Sphere));
    if (!sphere) {
        printf("Memory Allocation Error:: Failed to allocate new sphere");
        exit(1);
    }
    sphere->r = radius;
    sphere->pos = position;
    sphere->color = color;
    return sphere;
}



int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
    // Vector from the ray's origin to the sphere's center
    Vec3 V = subtract(rayPos, sphere->pos);

    // Calculate coefficients for the quadratic equation
    float a = dot(rayDir, rayDir); // Always 1 if rayDir is normalized
    float b = 2 * dot(rayDir, V);
    float c = dot(V, V) - sphere->r * sphere->r;

    // Calculate the discriminant
    float discriminant = b * b - 4 * a * c;

    // If the discriminant is negative, no intersection
    if (discriminant < 0) {
        return 0; // No intersection
    }

    // Compute the two solutions (roots) of the quadratic equation
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2 * a);
    float t2 = (-b + sqrtDiscriminant) / (2 * a);

    // Determine the closest valid intersection point
    if (t1 > 0 && t2 > 0) {
        *t = t1 < t2 ? t1 : t2; // Use the smaller positive t
        return 1;
    } else if (t1 > 0) {
        *t = t1; // Only t1 is valid
        return 1;
    } else if (t2 > 0) {
        *t = t2; // Only t2 is valid
        return 1;
    } else {
        return 0;
    }
    
}