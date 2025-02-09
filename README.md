# Raster image processing

## Introduction
### Description and idea of ​​the project
My project is a console application for processing Netpbm images, namely files in the **portable pixmap format (PPM)**, **portable graymap format (PGM)** and **portable bitmap format (PBM)** formats. Developed in **C++**, the idea of ​​the project is, using object-oriented programming techniques, to load files into objects of program classes. Once loaded into the program, image data can be modified using a set of functionalities.

### Goal and tasks of the development
One of the most important goals of the program is to work correctly. But in addition, an important goal for me was for it to work optimally, as well as to use as little device resources as possible. I have strived to separate the different functionalities into different functions, so that it would be easier in the future to edit and improve each of them.

Among the main tasks that I set myself when developing the application is to pay attention to all possible cases that may arise during the execution of code (for example, invalid data, non-existent files or files that cannot be opened). I tried to develop my program so that it does not terminate its work when a problem occurs, but instead tries to fix the error and if it fails to notify the user of the presence of a problem. In addition, I took it upon myself to come up with the algorithm of work myself instead of using someone else's.

### Structure of the documentation
In this documentation, I have selected some test inputs to demonstrate the operation of my program. In addition, I have explained my working methods and justified some of the choices I made when developing the code. Finally, the resources I used are indicated.

## Overview of the subject area
### Key definitions and concepts that will be used.
The principle by which images work can be well illustrated by their textual representations. Each pixel in a file has its own value. In **PPM** files, each pixel has three values ​​that specify the amount of a given color in the pixel – one for _red (R value)_, one for _green (G value)_, and one for _blue (B value)_. These values ​​range from 0 to the maximum value specified in the file (in PBM files, it is not specified in the text form of the file, but it cannot be a number other than 1).

The three formats have a similar text structure, but there are some key differences that require a slightly different approach to work. For example, in **.pbm** files there is no maximum value for pixels as in the other two formats; in .ppm, in addition to having a maximum pixel value, each pixel has a different value for red, green, and blue, while in the other two pixels have only one value each.

To edit an image, the information on its pixels must be changed. Possible cases are that the pixel values ​​need to be changed, their arrangement in the image needs to be changed, or some of the pixels need to be removed.
Problem definition and complexity of the task.

The problems that I expected to encounter when developing the application were the incorrect change of image information.

Initially, the complexity seemed high to me, because I did not understand how it is possible to edit a graphic file with code, but after I learned the principle of operation of Netpbm files, the complexity decreased significantly and different ideas for the implementation of the code began to come to me.

### Approaches and methods for solving the problems.

I solved the problem of incorrect change of values ​​with multiple checks and validation of the data that is written in the objects of a given class, as well as limited access to the data members from external classes.

I have not solved the problem of binary data in image files, because my program works only with text files written according to the model of the examples given in the Wikipedia page (link 1).

## Design
### General architecture – OOP design.
My code is based on the joint work of three classes:
• Class **Pixel** – This class is composed of data members that store the values ​​for the maximum value that can be set for each of the colors (red, green and blue), as well as the values ​​themselves for these three quantities; some operators that facilitate working with pixels; member functions for accessing the values ​​of the pixel data members; private member functions that ensure the validity of the data in the class.

• Class **Image** – The most important processes related to image editing occur in it. The function that this class performs is to store information about a given image in data structures that can be easily manipulated in the necessary way. Some of the data members are: a vector that stores Pixel class objects (which are mostly worked with later in the code); image height and widththe image and others. In addition, the class has member functions that perform a set of manipulations on the current image, as well as several private member functions, the purpose of which is better abstraction of the code.

• Class **Session** – It combines the functionalities of the Image and Pixel classes. It is composed of vectors that hold information about the images that are loaded in the current session, the indices of the images that need to be made into collages, the commands issued from the console and the commands that have been canceled (in case we want to restore them), as well as the coordinates of the points that we have submitted for cutting. In addition, the class has member functions that perform a set of commands on the session, as well as several private member functions, the purpose of which is better abstraction of the code.

• main_program – The final code is assembled here, uniting the three classes into one. It consists of checks validating user input, as well as a function that converts this code into an easy-to-use structure (vector). At the moment, only one session can work during the program execution.

## Implementation, testing.
Implementation of classes, memory management and algorithms. Optimizations.
• Class **Image** – For the implementation of the grayscale function, I used a formula from a page on the Internet (link 2). To convert images into monochrome (composed of only black or white pixels), I find the average value of the colors that make up the pixel and check whether it is closer to white (the maximum value) or black (value 0) in the image. For the negative function, in order for the images to take on their opposite values, for each color we need to change the value by the absolute difference between the maximum value and the current one. 
  To rotate and achieve a mirror image of the images, I use a temporary dynamic two-dimensional array, because indexing is convenient for such manipulation. When creating images that are collages, there are a total of six possible cases, which are implemented in a longer function that covers them all. Let heightDifference be the difference between the heights of the two images that make up the collage, in the following example. If the second image is taller, then one row of black pixels is written to the pixel vector of the collage, followed by one row of the second image. This is repeated (heightDifference / 2) times. Then one row of the first image is written, followed by one row of the second, and this is repeated until all rows of the first image are written. Finally, one row of black pixels is written again, followed by one row of the second image. This is repeated (heightDifference / 2) or (heightDifference / 2) + 1 times, depending on whether heightDifference is an even or odd number. 
    The approach when the first image is taller than the second is similar, just done in reverse. Let widthDifference be the difference between the widths of the two images composing the collage in the following example. For vertical collages of equal width, the approach is simple - first we write all the pixels from the first image into the collage pixel vector, then all the pixels from the second. If the second image is wider, then (widthDifference / 2) black pixels are written into the collage pixel vector, followed by a whole row of pixels from the first image. The collage row is padded with (widthDifference / 2) or (widthDifference / 2) + 1 black pixels, depending on whether widthDifference is an even or odd number. This is repeated for each row of the first image. Then we write all the pixels from the second image into the collage pixel vector. The approach when the first image is wider than the second is similar, just done in reverse. When cropping, it is possible to provide coordinates for the upper right or lower left point, as well as coordinates that do not form a rectangle (coincident coordinates), so I validate the data. After cropping, I shrink the capacity of the image pixel vector to use less system memory.

• Class **Session** – The implementation of this class is mostly data validation and optimization of commands issued from the console. For example, if the rotate left command is issued three times, the code will execute the rotate right command only once; images are edited only if the save command is issued, to save work if the command is missed; when executing the commands, the submitted crop commands will be executed first, to edit the smallest possible number of pixels, etc.
### A few test scenarios:
1:
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
2:
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
### Summary of the implementation:
The code optimization goals are not fully met, because there is always room for improvement, but I still think that the code does not burden the system too much and generally works well with the system resources. The code works even with incorrect input from the console.
### Directions for future development and improvement.
Future development of the program will mainly focus on expanding the range of file types it works with. In addition, I will develop the code so that it is compatible with different formats of text representations of images (for example, working with binary data and different orders). Also, I would look for more bugs that disrupt the correct operation of the program and come up with ways to deal with them.

## References
1. [Netpbm Wikipedia](https://en.wikipedia.org/wiki/Netpbm#File_formats)
2. [Grayscale Conversion Formula](https://learn.microsoft.com/en-us/previous-versions/bb332387(v=msdn.10)?redirectedfrom=MSDN#tbconimagecolorizer_grayscaleconversion)

