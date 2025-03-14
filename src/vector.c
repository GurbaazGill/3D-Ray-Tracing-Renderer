/***********************************************************/
/*      GURBAAZ GILL                                       */
/*      VECTOR.C FOR MILESTONE 1                           */
/*      Stack overflow used for ray tracing(not used)      */
/***********************************************************/

#include "vector.h"
#include <math.h>

// Adds two vectors and returns the result
Vec3 add(Vec3 v1, Vec3 v2) {
    Vec3 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return result;
}

// Subtracts vector v2 from vector v1 and returns the result
Vec3 subtract(Vec3 v1, Vec3 v2) {
    Vec3 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    return result;
}

// Multiplies a vector by a scalar and returns the result
Vec3 scalarMultiply(float s, Vec3 v) {
    Vec3 result = {s * v.x, s * v.y, s * v.z};
    return result;
}

// Divides a vector by a scalar and returns the result
Vec3 scalarDivide(Vec3 v, float d) {
    if (d != 0) {  // Avoid division by zero
        Vec3 result = {v.x / d, v.y / d, v.z / d};
        return result;
    }
    return (Vec3){0.0, 0.0, 0.0};  // Return zero vector if division by zero
}

// Normalizes the vector (returns a unit vector in the same direction)
Vec3 normalize(Vec3 v) {
    float len = length(v);
    return scalarDivide(v, len);
}

// Returns the dot product of two vectors
float dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Calculates the length (magnitude) of a vector
float length(Vec3 v) {
    return sqrt(length2(v));
}

// Calculates the distance between two vectors
float distance(Vec3 v1, Vec3 v2) {
    return sqrt(distance2(v1, v2));
}

// Calculates the length (magnitude) of a vector
float length2(Vec3 v) {
    return dot(v, v);
}

// Calculates the distance between two vectors
float distance2(Vec3 v1, Vec3 v2) {
    Vec3 diff = subtract(v1, v2);
    return length2(diff);
}
