#include "color.h"
#include <math.h>


// Function to unpack the HEX value and return the corresponding RGB [0, 1] values
Vec3 unpackRGB(unsigned int packedRGB) {
    // Extract the red, green, and blue components (each is 8 bits)
    unsigned int red = (packedRGB >> 16) & 0xFF;   // Shift right by 16 bits to get the red part
    unsigned int green = (packedRGB >> 8) & 0xFF;   // Shift right by 8 bits to get the green part
    unsigned int blue = packedRGB & 0xFF;           // Mask the last 8 bits to get the blue part

    // Scale each component from the range [0, 255] to [0, 1]
    Vec3 result;
    result.x = (float) red / 255.0f;   // Red in the range [0, 1]
    result.y = (float) green / 255.0f; // Green in the range [0, 1]
    result.z = (float) blue / 255.0f;  // Blue in the range [0, 1]

    return result;
}

// FUNCTION TO WRITE COLOR IN PPM FILE
void writeColour(FILE *ppmFile, Vec3 color) {
    
    // Convert to integer range [0, 255]
    int ir = (color.x *255.0f);
    int ig = (color.y * 255.0f);
    int ib = (color.z * 255.0f);

    // Write the color to the file
    fprintf(ppmFile, "%d %d %d\n", ir, ig, ib);
}

// CODE FOR COLOR COMPARISON GIVEN ON GITHUB
int compareColor(const void *a, const void *b) {
    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++) {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }

    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*(int*)a < *(int*)b) ? -1 : (*(int*)a > *(int*)b);
}