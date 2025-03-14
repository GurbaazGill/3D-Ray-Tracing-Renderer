# Ray Tracing Engine

## Overview
This project is a **Ray Tracing Engine** built from scratch in **C**, designed to generate realistic 3D images using **ray tracing techniques**. The engine supports fundamental graphics programming concepts such as **dynamic memory allocation, file I/O, low-level programming, string manipulation, and sorting (qSort)**.

This project was built to deepen my understanding of **computer graphics, rendering pipelines, and low-level C programming** while exploring the challenges of developing a ray tracer without relying on existing graphics engines.

---

## Features
- **3D Rendering Engine**: Generates a 3D image of spheres in space using ray tracing.
- **Vector Mathematics**: Implements custom **vector operations** for accurate geometric calculations.
- **Ray-Sphere Intersection**: Uses mathematical models to detect ray intersections with spheres.
- **Lighting & Shading**: Implements point lighting and shading techniques for realistic rendering.
- **Dynamic Memory Management**: Efficiently stores world objects using **dynamic arrays**.
- **Color Handling & Sorting**: Parses colors from **HEX format**, converts them to **RGB**, and sorts them using **qSort**.
- **Anti-Aliasing**: Improves image quality by **smoothing edges**.
- **PPM File Output**: Saves the rendered image in **Portable Pixmap Format (PPM)** for easy viewing.

---

## Implementation Details
### 1. Ray Tracing Fundamentals
The core rendering pipeline follows these steps:
1. **Ray Generation**: A ray is cast from the camera for each pixel in the image.
2. **Intersection Testing**: The ray checks for intersections with objects (spheres).
3. **Lighting Calculation**: If an intersection occurs, the engine calculates shading based on light intensity and object color.
4. **Shadow Casting**: Additional rays determine if the intersection point is in shadow.
5. **Color Assignment**: The final pixel color is determined and written to the output image file.

### 2. Multi-File Structure
The project follows a **modular** approach, separating concerns into multiple C files:
```
src/
|-- main.c            # Main program logic
|-- vector.c          # Vector math operations
|-- spheres.c         # Sphere intersection logic
|-- color.c           # Color conversion and handling
|-- rendering.c       # Ray tracing and lighting calculations
|-- Makefile          # Build system
```

### 3. Vector Mathematics
A custom **Vec3** structure is used for 3D vector calculations, including:
- **Addition & Subtraction**
- **Scalar Multiplication & Division**
- **Normalization & Length Calculation**
- **Dot Product & Distance Calculation**

### 4. Sphere Representation & Intersection Testing
Each sphere is defined with:
- **Position (Vec3)**
- **Radius (float)**
- **Color (Vec3 - RGB format)**

The engine calculates ray-sphere intersections using quadratic equations and ensures efficient handling of multiple objects in the scene.

### 5. Lighting & Shadows
- Implements **Phong-based shading** with a single light source.
- Uses **shadow rays** to detect occlusions and render realistic shadows.
- Light intensity diminishes based on the **distance from the source**.

### 6. Color Handling & PPM Output
- Colors are read in **HEX format**, converted to **RGB**, and stored in a **sorted color palette**.
- The output is saved in **PPM format**, allowing easy visualization with PPM-compatible viewers.

### 7. Anti-Aliasing
- Uses **grid-based sampling** to smooth jagged edges in the final render.
- Each pixel is sampled **9 times**, averaging colors for a more refined image.

---

## Build & Run Instructions
### Prerequisites
- **GCC Compiler** (or any C compiler)
- **Make** (optional but recommended)

### Compilation
Use the provided `Makefile` to compile the project:
```
make
```
This generates an executable `raytracer` in the root directory.

### Running the Program
```
./raytracer <input_file> <output_file.ppm>
```
Example:
```
./raytracer scenes/sample_scene.txt output.ppm
```

### Viewing the Output
You can view the generated `.ppm` image using:
- **Linux/macOS**: Open with `eog` or convert to PNG using `convert output.ppm output.png`
- **Windows**: Use **IrfanView** or **GIMP**
- **VS Code**: Install [PPM Viewer Extension](https://marketplace.visualstudio.com/items?itemName=ngtystr.ppm-pgm-viewer-for-vscode)

---

## Example Output
### **Rendered Image (Final Submission Example)**
![Rendered Output](assets/rendered_output.png)

---

## Future Improvements
- **Reflections & Refractions**: Extend ray tracing to handle mirror-like reflections and transparent objects.
- **Multiple Light Sources**: Implement multiple lights for complex illumination.
- **BVH Acceleration**: Improve efficiency with **Bounding Volume Hierarchies**.
- **Texture Mapping**: Allow surface textures to be applied to objects.

---

## License
This project is **open-source** and available under the **MIT License**.

---

## Contact
If you have any questions or suggestions, feel free to reach out:
- GitHub: [GurbaazGill](https://github.com/GurbaazGill)
- Email: gurbaaz@ualberta.ca

