#pragma once
#include <iostream>

/* At the core of the program's functionality lies the handling of text files,
specifically reading and writing information about the pixels that make up
a given image. Pixel is a class designed to simplify working with individual pixels. */

class Pixel
{
private:
	// For the class member variables, I chose the most important characteristics of the pixels, namely:
	unsigned short maxValue; // The maximum value that can be assigned to one of the colors
	// that make up the pixel and define its final appearance

	unsigned short red; // The value of the red color in the pixel
	unsigned short green; // The value of the green color in the pixel
	unsigned short blue; // The value of the blue color in the pixel
public:
	// Default constructor, parameterized constructor, and destructor of the class:
	Pixel();
	Pixel(const unsigned short& maxValue, const unsigned short& red, const unsigned short& green, const unsigned short& blue);
	~Pixel();

	// Operators that simplify working with pixels:
	Pixel& operator=(const Pixel&);
	bool operator==(const Pixel&);
	bool operator!=(const Pixel&);

	// Member functions for accessing the values of the pixel's member variables:
	short getMaxValue() const;
	short getRValue() const;
	short getGValue() const;
	short getBValue() const;
private:
	// Private member functions that ensure the validity of the data in the class:
	void setMaxValue(const unsigned short&);
	void setValues(const unsigned short&, const unsigned short&, const unsigned short&);
};