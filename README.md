# Raster Image Processing

## Introduction

### Description and Idea
This project is a **console application** for processing **Netpbm images**, specifically files in:
- **PPM (Portable Pixmap Format)**
- **PGM (Portable Graymap Format)**
- **PBM (Portable Bitmap Format)**

Developed in **C++**, the project leverages **Object-Oriented Programming (OOP)** principles to load images into structured objects. Once loaded, images can be modified using a variety of functionalities.

### Goals and Objectives
- **Correctness**: Ensuring the program processes images accurately.
- **Efficiency**: Optimizing resource usage and execution speed.
- **Robustness**: Handling errors such as invalid data, missing files, or read/write issues.
- **Code Modularity**: Separating functionalities into distinct functions to improve maintainability.

Rather than relying on existing implementations, the project employs **custom-built algorithms** for image processing.

### Documentation Structure
- **Test Inputs**: Sample inputs demonstrating program functionality.
- **Implementation Details**: Explanation of design choices and methodologies.
- **Resources**: References used during development.

## Overview

### Key Concepts
Each image is represented as a collection of **pixels**, where:
- **PPM**: Each pixel has three values (**R, G, B**) ranging from `0` to a max value.
- **PGM**: Each pixel has a **single grayscale value**.
- **PBM**: Each pixel is either **black (0) or white (1)**.

### Challenges and Complexity
Initially, modifying graphic files via code seemed complex. However, after understanding **Netpbm file structures**, the process became more intuitive. The primary challenge was ensuring **correct pixel manipulation**.

### Approach and Solutions
- **Data Validation**: Multiple checks to prevent invalid image modifications.
- **Encapsulation**: Restricted direct data access to ensure integrity.
- **Text-based Processing**: Currently, the program does not handle binary data but follows the text format from [Netpbm Wikipedia](https://en.wikipedia.org/wiki/Netpbm#File_formats).

## Design

### Architecture - Object-Oriented Design
The project consists of **three core classes**:

| Class | Description |
|-------|-------------|
| **Pixel** | Stores pixel values (R, G, B), ensures validity, and provides operators for manipulation. |
| **Image** | Manages image data, provides pixel editing functions, and abstracts implementation details. |
| **Session** | Handles multiple images, user commands, undo/redo actions, and cropping coordinates. |

The **main program** integrates these classes, handles user input, and organizes the workflow. Only one session can be active at a time.

## Implementation and Testing

### Key Implementations
#### Image Class
- **Grayscale Conversion**: Uses a formula from a page on the Internet (link 2).
- **Monochrome Conversion**: Maps pixel values to black or white based on an average threshold.
- **Negative Effect**: Inverts color values relative to their maximum.
- **Rotation and Flipping**: Uses a temporary dynamic array for efficient manipulation.
- **Collage Creation**: Implements six different collage scenarios based on image dimensions.
- **Cropping**: Ensures valid rectangle formation and optimizes memory usage.

#### Session Class
- **Command Optimization**: For example, three consecutive `rotate left` commands execute as `rotate right` once.
- **Lazy Processing**: Images are modified only when `save` is executed.
- **Batch Execution**: Crop commands are prioritized for efficiency.

### Test Scenarios
#### Scenario 1: Basic Image Editing
```
load long_rgb.ppm rgb.ppm
make collage vertical long_rgb.ppm rgb.ppm
grayscale
flip horizontal
flip horizontal
flip vertical
rotate left
rotate left
rotate left
save
exit
```

#### Scenario 2: Monochrome and Cropping
```
load j.pbm j2.pgm
monochrome
crop 2 5 7 2
add rgb.ppm
negative
session info
undo
redo
save
exit
```

## Conclusion
### Summary
- The program successfully processes Netpbm images with **optimized performance**.
- It handles **incorrect user input gracefully**.
- The modular architecture makes it **extensible and maintainable**.

### Future Enhancements
- **Support for Binary Formats**: Expanding compatibility beyond text-based Netpbm files.
- **Advanced Image Filters**: Implementing effects like blur, sharpen, and edge detection.
- **Bug Fixes and Optimization**: Continuous improvement of performance and stability.

## References
1. [Netpbm Wikipedia](https://en.wikipedia.org/wiki/Netpbm#File_formats)
2. [Grayscale Conversion Formula](https://learn.microsoft.com/en-us/previous-versions/bb332387(v=msdn.10)?redirectedfrom=MSDN#tbconimagecolorizer_grayscaleconversion)

