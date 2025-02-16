#pragma once
#include <fstream>
#include <string>
#include "Pixel.h"
#include <vector>

/* The most important processes related to image editing take place here, in the Image class.
The idea behind this class is to store information about a given image in data structures
that can be easily manipulated as needed. */

class Image
{
protected:
	// The member variables of the class are as follows:
	char magicNumber[3]; // An array of characters that contains the information with which every Netpbm file starts 
						// in text form, namely P<digit from 1 to 6>, which defines the file format 
						// (portable pixmap format (PPM), portable graymap format (PGM), 
						// portable bitmap format (PBM)). I decided to keep it because there are two possibilities 
						// for the digit after P for each format.

	std::string filePath; // This contains the information about the file's location and name
	std::string fileExtension; // This contains the information about the file extension. Although I can already
								// determine the file format from the magicNumber, this attribute saves a few checks later
								// in the code.
	unsigned short width; // This contains the information about the width of the file
	unsigned short height; // This contains the information about the height of the file
	std::vector<Pixel> pixels; // The member variables width and height are important for the number of pixels
								// that will be stored in the vector pixels
	unsigned short commandsToSkip; // This contains information necessary for executing commands
									// in the main code. The need for this variable arises from the fact that
									// images can be added to a session at a later stage without applying the previous
									// commands to them.

public:
	// Constructors
	Image();
	Image(const std::string& filePath, const unsigned short& commandsToSkip = 0);
	~Image();

	
	unsigned short getCommandsToSkip() const;
	std::string getFilePath() const;
	std::string getFileExtension() const;
	void setFilePath(const std::string& filePath);

	void loadImage(const std::string&);
	void saveImage();

	// Member functions that perform manipulations on the current image:
	void toGrayscale();
	void toMonochrome();
	void toNegative();
	void rotateLeft();
	void rotateRight();
	void flipHorizontal();
	void flipVertical();
	void crop(unsigned short xTL, unsigned short yTL, unsigned short xBR, unsigned short yBR);
	friend Image makeCollage(const std::string& orientation, const Image& img1, const Image& img2);
private:
	// Helper member functions that facilitate loading and saving the image
	void loadPBMAndPGM(std::ifstream&, const short&);
	void loadPPM(std::ifstream&, const short&);
	std::string getNewFileName();
};

