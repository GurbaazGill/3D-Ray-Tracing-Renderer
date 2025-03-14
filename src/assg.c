//      GURBAAZ GILL
//      ASSG.C FOR FINAL SUBMISSION
//      Stack overflow used for ray tracing

#include "assg.h"

/////////////////////////////////////////////////   MAIN FUNCTION   ///////////////////////////////////////////////////

int main(int argc, char const *argv[]) {
    // Makes sure that the numer of arguments is valif
    if (argc != 3) {
        printf("Not the correct number of arguments\n");
        return 1;
    } 
    // Reading input and output file paths 
    const char *inputFilePath = argv[1];
    const char *outputFilePath = argv[2];
    

    // Opening input file to read and output file to write
    FILE *inputFile = fopen(inputFilePath, "r");
    if (!inputFile) {
        printf("Unable to open input file\n");
        exit(-1);
    }

    FILE *outputFile = fopen(outputFilePath, "w");
    if (!outputFile) {
        printf("Unable to open output file\n");
        fclose(inputFile);
        exit(-1);
    }


///////////////////////////////////////////////////////   INPUT HANDLING    ///////////////////////////////////////////////////////

    // Initializing ariables for storing input data
    int imageWidth, imageHeight, BGColorIndex;
    int numColors;
    float viewportHeight, focalLength, lightIntensity;
    Vec3 lightPosition;
    int numSpheres;

    // Read the viewport dimensions, viewport height, and focal length
    if (fscanf(inputFile, "%d %d", &imageWidth, &imageHeight) != 2) {
        printf("\nError: Failed to read input from input file");
        exit(1);
    }
    if (fscanf(inputFile, "%f", &viewportHeight) != 1) {
        printf("\nError: Failed to read input from input file");
        exit(1);
    }
    if (fscanf(inputFile, "%f", &focalLength) != 1) {
        printf("\nError: Failed to read input from input file");
        exit(1);
    }

    // Calculating viewport width according to given aspect ratio and creating viewport
    float viewportWidth = viewportHeight * (float)imageWidth / (float)imageHeight;
    Viewport viewport = createViewport(viewportWidth, viewportHeight, focalLength);

    // Read the light position and intensity and creating light 
    if (fscanf(inputFile, "%f %f %f %f", &lightPosition.x, &lightPosition.y, &lightPosition.z, &lightIntensity) != 4) {
        printf("\nError: Failed to read input from input file");
        exit(1);
    }

    Light light = createLight(lightPosition, lightIntensity);

    // reading number of colors and initializing the araray
    if (fscanf(inputFile, "%d", &numColors) != 1) {
        printf("\nError: Failed to read input from input file");
        exit(1);
    }
    unsigned int HexadecimalColors[numColors];

    // storing the colors in the arra y - given in input file
    for (int i = 0; i < numColors; i++) {
        if (fscanf(inputFile, "%x", &HexadecimalColors[i]) != 1) {
            printf("\nError: Failed to read input from input file");
            exit(1);
        }    
    }
    qsort(HexadecimalColors, numColors, sizeof(unsigned int), compareColor);

    // Reading the index of Background color and hardcording it to black for milestone2
    if (fscanf(inputFile, "%d", &BGColorIndex) != 1) {
        printf("\nError: Failed to read input from input file");
        exit(1);
    }
    #ifdef FS
    Vec3 backgroundColor = unpackRGB(HexadecimalColors[BGColorIndex]);
    #else
    Vec3 backgroundColor = (Vec3){0.0, 0.0, 0.0};
    #endif

    // Initialize the world to store spheres
    World world;
    worldInit(&world);

    // Read the number of spheres and create spheres
    if (fscanf(inputFile, "%d", &numSpheres) != 1) {
        printf("\nError: Failed to read input from input file");
        exit(1);
    }   

    // Read each sphere's detaiLs and create the sphhere
    for (int i = 0; i < numSpheres; i++) {
        Vec3 position;
        float radius;
        int colorIndex;
        if (fscanf(inputFile, "%f %f %f %f %d", &position.x, &position.y, &position.z, &radius, &colorIndex) != 5) {
            printf("\nError: Failed to read input from input file");
            exit(1);
        }

    #ifndef FS
        Vec3 sphereColor = {1.0, 1.0, 1.0}; // Default color for spheres is white
    #else 
        Vec3 sphereColor = unpackRGB(HexadecimalColors[colorIndex]);
    #endif

        Sphere *sphere = createSphere(radius, position, sphereColor);
        addSphere(&world, sphere);
    }

    // closing input file since all values have been read
    fclose(inputFile);


///////////////////////////////////////////////////////     OUTPUT HANDLING     ///////////////////////////////////////////////////////
#ifdef FS
    renderSceneFinal(outputFile, viewport, imageWidth, imageHeight, &world, backgroundColor, light);
#elif MS2
    renderSceneMilestones(outputFile, viewport, imageWidth, imageHeight, &world, backgroundColor, light);
#else
    OutputMilestoneOne(outputFile, backgroundColor, light, viewport, world);
#endif

    freeWorld(&world);
    fclose(outputFile);
    return 0;
}

        
//////////////////////////////////////////////////////      CREATING REQUIRED FUNCTIONS     //////////////////////////////////////////////////////
Viewport createViewport(float width, float height, float focalLength) {
    Viewport viewport = {width, height, focalLength};
    return viewport;
}

Light createLight(Vec3 position, float lightIntensity) {
    Light light;
    light.position = position;
    light.intensity = lightIntensity; 
    return light;
}

Ray createRay(Vec3 origin, Vec3 direction) {
    Ray ray = {origin, direction};
    return ray;
}

Vec3 computeLighting(Vec3 intersectionPoint, Vec3 normal, Light light, World *world, Sphere *closestSphere) {
    Vec3 lightDirection = normalize(subtract(light.position, intersectionPoint));
    float dotProd = (dot(normal, lightDirection) > 0.0) ? dot(normal, lightDirection) : 0.0;

    float light_distance = distance2(light.position, intersectionPoint);
    float intensity = (light.intensity * (dotProd/light_distance) < 1.0) ? light.intensity * (dotProd/light_distance) : 1.0;
    Vec3 pixelColor = scalarMultiply(intensity, closestSphere->color);

    Ray ShadowRay;
    ShadowRay.origin =  add(intersectionPoint, scalarMultiply(0.001f,normal));
    ShadowRay.direction = lightDirection;

    float distance = distance2(light.position, ShadowRay.origin);

    float shadowT;
    for (int i = 0; i < world->size; i++) {
        if (doesIntersect(world->spheres[i], ShadowRay.origin, lightDirection, &shadowT)) {
            if (shadowT > 0 && distance > pow(shadowT, 2)) {
                return scalarMultiply(SHADOW_FACTOR, pixelColor);
            }
        }
    } 
    return pixelColor;
}

// Rendering pipeline for milestone 2
void renderSceneMilestones(FILE *outputFile, Viewport viewport, int imageWidth, int imageHeight, World *world, Vec3 backgroundColor, Light light) {
    // Write the PPM header and initializing camera
    fprintf(outputFile, "P3\n%d %d\n255\n", imageWidth, imageHeight);
    Vec3 camera = {0.0, 0.0, 0.0};
    Vec3 pixelColor;

    // Loop through each pixel
    for(int y = imageHeight - 1; y >= 0; y--) {
        for (int x = 0; x < imageWidth; x++) {

            // Map pixel coordinates to viewport
            float u = ((x + 0.5) - (imageWidth / 2.0)) * (viewport.width / imageWidth);
            float v = ((y + 0.5) - (imageHeight / 2.0)) * (viewport.height / imageHeight);
        
            // Directing ray from camera to viewport
            Ray CameraRay;
            CameraRay.origin = (Vec3){0.0, 0.0, 0.0}; // Camera position
            CameraRay.direction = normalize(subtract((Vec3){u, v, -viewport.focalLength}, CameraRay.origin)); // Direction vector
            
            // Initialize variables for closest intersection
            float closestT = __FLT_MAX__;
            Sphere *closestSphere = NULL;

            // Check intersection with all spheres
            for (int i = 0; i < world->size; i++) {
                Sphere *sphere = world->spheres[i];
                float t;
                if (doesIntersect(sphere, camera, CameraRay.direction, &t) && t < closestT) {
                    closestT = t;
                    closestSphere = sphere;
                }
            }

            if (closestSphere) {
                // Compute intersection point
                Vec3 intersectionPoint = add(CameraRay.origin, scalarMultiply(closestT, CameraRay.direction));
                // Compute surface normal at intersection
                Vec3 normal = normalize(subtract(intersectionPoint, closestSphere->pos));
                // Compute lighting
                pixelColor = computeLighting(intersectionPoint, normal, light, world, closestSphere);
            } else {
                // Use background color if no intersection
                pixelColor = backgroundColor;
            }
            // Write color to PPM file
            writeColour(outputFile, pixelColor);
        }
    }
}



// Rendering pipeline for final
void renderSceneFinal(FILE *outputFile, Viewport viewport, int imageWidth, int imageHeight, World *world, Vec3 backgroundColor, Light light) {

    // Write the PPM header
    fprintf(outputFile, "P3\n%d %d\n255\n", imageWidth, imageHeight);

    // iNITIALIZING cAMERA
    Vec3 camera = {0.0, 0.0, 0.0};

    // Loop through each pixel
    for(int y = imageHeight - 1; y >= 0; y--) {
        for (int x = 0; x < imageWidth; x++) {
            
            Vec3 pixelColor = (Vec3){0.0, 0.0, 0.0};
            float movements[3] = {-1.0/3.0, 0.0, 1.0/3.0};
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    // Map pixel coordinates to viewport
                    float u = ((x + 0.5 + movements[i] ) - (imageWidth / 2.0)) * (viewport.width / imageWidth);
                    float v = ((y + 0.5 + movements[j]) - (imageHeight / 2.0)) * (viewport.height / imageHeight);
                
                    // Directing ray from camera to viewport
                    Ray CameraRay;
                    CameraRay.origin = (Vec3){0.0, 0.0, 0.0}; // Camera position
                    CameraRay.direction = normalize(subtract((Vec3){u, v, -viewport.focalLength}, CameraRay.origin)); // Direction vector
                    
                    // Initialize variables for closest intersection
                    float closestT = __FLT_MAX__;
                    Sphere *closestSphere = NULL;

                    // Check intersection with all spheres
                    for (int i = 0; i < world->size; i++) {
                        Sphere *sphere = world->spheres[i];
                        float t;
                        if (doesIntersect(sphere, camera, CameraRay.direction, &t) && t < closestT) {
                            closestT = t;
                            closestSphere = sphere;
                        }
                    }

                    Vec3 NinthPixelColor;
                    if (closestSphere) {
                        // Compute intersection point
                        Vec3 intersectionPoint = add(CameraRay.origin, scalarMultiply(closestT, CameraRay.direction));
                        // Compute surface normal at intersection
                        Vec3 normal = normalize(subtract(intersectionPoint, closestSphere->pos));
                        // Compute lighting
                        NinthPixelColor = computeLighting(intersectionPoint, normal, light, world, closestSphere);
                    } else {
                        // Use background color if no intersection
                        NinthPixelColor = backgroundColor;
                    }
                    pixelColor = add(pixelColor, NinthPixelColor);
                }
            }
            pixelColor = scalarDivide(pixelColor, 9.0);
            // Write color to PPM file
            writeColour(outputFile, pixelColor);
        }
    }
}

// Output For Milestone1
void OutputMilestoneOne(FILE *outputFile, Vec3 backgroundColor, Light light, Viewport viewport, World world) {
    Vec3 addResult = add(backgroundColor, light.position);
    Vec3 subtractResult = subtract(backgroundColor, light.position);
    Vec3 scalarMultiplyResult = scalarMultiply(viewport.width, light.position);
    Vec3 normalizeResult = normalize(light.position);

    fprintf(outputFile, "(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            backgroundColor.x, backgroundColor.y, backgroundColor.z,
            light.position.x, light.position.y, light.position.z,
            addResult.x, addResult.y, addResult.z);
    fprintf(outputFile, "(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            backgroundColor.x, backgroundColor.y, backgroundColor.z,
            light.position.x, light.position.y, light.position.z,
            subtractResult.x, subtractResult.y, subtractResult.z);
    fprintf(outputFile, "%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            viewport.width,
            light.position.x, light.position.y, light.position.z,
            scalarMultiplyResult.x, scalarMultiplyResult.y, scalarMultiplyResult.z);
    fprintf(outputFile, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            light.position.x, light.position.y, light.position.z,
            normalizeResult.x, normalizeResult.y, normalizeResult.z);

    // Output for each sphere
    for (int i = 0; i < world.size; i++) {
        Sphere *sphere = world.spheres[i];
        Vec3 scalarDivideResult = scalarDivide(sphere->color, sphere->r);
        float dotResult = dot(light.position, sphere->pos);
        float distanceResult = distance(light.position, sphere->pos);
        float lengthResult = length(sphere->pos);
        // New line before each sphere
        fprintf(outputFile, "\n");
        // Sphere color / sphere radius
        fprintf(outputFile, "(%.1f, %.1f, %.1f) / %.1f = (%.1f, %.1f, %.1f)\n",
                sphere->color.x, sphere->color.y, sphere->color.z,
                sphere->r,
                scalarDivideResult.x, scalarDivideResult.y, scalarDivideResult.z);
        // Dot product
        fprintf(outputFile, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
                light.position.x, light.position.y, light.position.z,
                sphere->pos.x, sphere->pos.y, sphere->pos.z,
                dotResult);
        // Distance
        fprintf(outputFile, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
                light.position.x, light.position.y, light.position.z,
                sphere->pos.x, sphere->pos.y, sphere->pos.z,
                distanceResult);
        // Length
        fprintf(outputFile, "length(%.1f, %.1f, %.1f) = %.1f\n",
                sphere->pos.x, sphere->pos.y, sphere->pos.z,
                lengthResult);
    }
}