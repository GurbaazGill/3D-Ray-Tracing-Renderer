#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "spheres.h"
#include "color.h"
#define SHADOW_FACTOR 0.1f

// Structures for viewport
typedef struct {
    float width;
    float height;
    float focalLength;
} Viewport;

// Structures for ray
typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

// Structures for light 
typedef struct {
    Vec3 position;
    float intensity;
} Light;

Viewport createViewport(float width, float height, float focalLength);
Light createLight(Vec3 position, float lightIntensity);
Ray createRay(Vec3 origin, Vec3 direction);
Vec3 computeLighting(Vec3 intersectionPoint, Vec3 normal, Light light, World *world, Sphere *closestSphere);
void renderSceneMilestones(FILE *outputFile, Viewport viewport, int imageWidth, int imageHeight, World *world, Vec3 backgroundColor, Light light);
void renderSceneFinal(FILE *outputFile, Viewport viewport, int imageWidth, int imageHeight, World *world, Vec3 backgroundColor, Light light);
void OutputMilestoneOne(FILE *outputFile, Vec3 backgroundColor, Light light, Viewport viewport, World world);
