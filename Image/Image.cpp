#include "Image.h"
#include <regex>

// Implementation of constructor and access member functions
Image::Image() : magicNumber{ }, fileExtension(""), filePath(""), width(0), height(0), commandsToSkip(0) { }

Image::Image(const std::string& filePath, const unsigned short& commandsToSkip)
{
	loadImage(filePath);
	this->commandsToSkip = commandsToSkip;
}

Image::~Image() { }

unsigned short Image::getCommandsToSkip() const
{
	return this->commandsToSkip;
}

std::string Image::getFilePath() const
{
	return this->filePath;
}

std::string Image::getFileExtension() const
{
	return this->fileExtension;
}

void Image::setFilePath(const std::string& filePath) {
	std::regex path(R"(^([a-zA-Z]:\\|/)?(([^<>:"/\\|?*]+[/\\])*[^<>:"/\\|?*]+)?$)");
	if (!filePath.empty() && std::regex_match(filePath, path))
	{
		this->filePath = filePath;
	}
}

// The approach to loading the data of an image into an object of the Image class is based on reading 
// the information from the text format of the images using an input stream.

void Image::loadImage(const std::string& filePath)
{
	std::ifstream is(filePath);
	if (!is.is_open())
	{
		std::cout << "Could not open file " << filePath << "\n";
		return;
	}

	this->filePath = filePath;
	this->filePath.erase(filePath.size() - 4);
	this->fileExtension = filePath.substr(filePath.length() - 4, 4);
	// This program works only with Netpbm, so the following check is necessary:
	if (this->fileExtension != ".pbm" && this->fileExtension != ".pgm" && this->fileExtension != ".ppm")
	{
		std::cout << "Could not recognize the file format\n";
		return;
	}

	do
	{
		is.getline(this->magicNumber, 3);
		this->magicNumber[2] = '\0';
	} while (this->magicNumber[0] == '#'); // Sometimes the text format of the files contains comments that start with the '#' character. 

	char buffer[26];
	do
	{
		is.getline(buffer, 26);
	} while (buffer[0] == '#');

	size_t i = 0;
	while (buffer[i] != ' ')
	{
		(this->width *= 10) += (int)buffer[i] - 48; //Changing text into a number
		i++;
	}
	i++;
	while (buffer[i] != '\0')
	{
		(this->height *= 10) += (int)buffer[i] - 48;
		i++;
	}
	unsigned short maxValue = 0;

	// The three formats have a similar text structure, but there are some key differences that require a slightly different 
	// approach. For example, in .pbm files there is no maximum value for the pixels as there is in the other two formats; 
	// in .ppm, in addition to having a maximum pixel value, each pixel has different values for red, green, and blue 
	// colors, whereas in the other two formats, the pixels have only one value each.
	if (this->fileExtension == ".pbm")
	{
		loadPBMAndPGM(is, 1);
	}
	else
	{
		is.getline(buffer, 26);

		for (size_t i = 0; buffer[i] != '\0'; i++)
		{
			(maxValue *= 10) += (int)buffer[i] - 48;
		}

		if (this->fileExtension == ".pgm")
		{
			loadPBMAndPGM(is, maxValue);
		}
		else
		{
			loadPPM(is, maxValue);
		}
	}
	this->pixels.shrink_to_fit();
}


// Just like when reading, when writing files we use a stream, but this time for output.
void Image::saveImage()
{
	std::string newFilePath = getNewFileName();

	std::ofstream os(newFilePath);
	if (!os.is_open())
	{
		std::cout << "Could not open file " << newFilePath << "\n";
		return;
	}

	os << this->magicNumber << "\r";
	os << this->width << " " << this->height << "\r";

	// Just like when reading, the structure of the files is similar, 
	// but the aforementioned differences must be observed 
	if (this->fileExtension == ".pbm" || this->fileExtension == ".pgm")
	{
		if (this->fileExtension == ".pgm")
		{
			os << this->pixels[0].getMaxValue() << "\r";
		}
		for (size_t i = 0; i < pixels.size(); i++)
		{
			os << pixels[i].getRValue();
			if ((i + 1) % width == 0)
			{
				os << "\r";
			}
			else
			{
				os << " ";
			}
		}
	}
	else
	{
		os << this->pixels[0].getMaxValue() << "\r";
		for (size_t i = 0; i < pixels.size(); i++)
		{
			os << pixels[i].getRValue() << " " << pixels[i].getGValue() << " " << pixels[i].getBValue() << "\r";
		}
	}
}

// Since the only difference in the text format of .pbm and .pgm is whether the pixels have a maximum value or not,
// I combined the reading of the two files into one function. Although the pixels in .pbm do not have a specified maximum value in 
// the documentation, it is always 1.
void Image::loadPBMAndPGM(std::ifstream& is, const short& maxValue)
{
	for (size_t i = 0; i < this->height; i++)
	{
		unsigned int size = this->width * 4;// In .pgm, we have a maximum of width times three-digit values for the pixels + (width - 1) times space between them + one '\0' character.
		char* buffer = new char[size];
		unsigned short pixelValue = 0;
		is.getline(buffer, size);
		for (size_t j = 0; j < size; j++)
		{
			while (buffer[j] != ' ' && buffer[j] != '\0')
			{
				(pixelValue *= 10) += (int)buffer[j] - 48;
				j++;
			}
			Pixel pixel(maxValue, pixelValue, pixelValue, pixelValue);
			pixels.push_back(pixel);
			pixelValue = 0;
			if (buffer[j] == '\0')
			{
				break;
			}
		}
		delete[] buffer;
	}
}

void Image::loadPPM(std::ifstream& is, const short& maxValue)
{
	const unsigned int pixelCount = this->height * this->width;
	for (size_t i = 0; i < pixelCount; i++)
	{
		const unsigned int buffSize = 27; // In .ppm, we have a maximum of width times three-digit values for the pixels + (width - 1) times space between them + one '\0' character.
		char* buffer = new char[buffSize];
		unsigned short values[3] = { 0,0,0 };
		size_t t = 0;
		is.getline(buffer, buffSize);
		for (size_t j = 0; buffer[j] != '\0'; j++)
		{
			if (buffer[j] == ' ')
			{
				continue;
			}
			while (buffer[j] != ' ' && buffer[j] != '\0')
			{
				(values[t] *= 10) += (int)buffer[j] - 48;
				j++;
			}
			t++;
			if (buffer[j] == '\0')
			{
				break;
			}
		}

		Pixel pixel(maxValue, values[0], values[1], values[2]);
		pixels.push_back(pixel);
		delete[] buffer;
	}
}

std::string Image::getNewFileName()
{
	unsigned long long currentTime = std::time(nullptr); // I use a long long variable to prevent data loss
	std::string newFileName = this->filePath;
	newFileName.append("_").append(std::to_string(currentTime));
	newFileName += this->fileExtension;
	return newFileName;
}

void Image::toGrayscale()
{
	if (this->fileExtension == ".pbm" || this->fileExtension == ".pgm")
	{
		return;
	}
	size_t imgSize = this->width * this->height;
	for (size_t i = 0; i < imgSize; i++)
	{
		// The following formula for achieving the grayscale appearance of the image is taken from:
		//https://learn.microsoft.com/en-us/previous-versions/bb332387(v=msdn.10)?redirectedfrom=MSDN#tbconimagecolorizer_grayscaleconversion

		unsigned short grayValue = 0.299 * this->pixels[i].getRValue() + 0.587 * this->pixels[i].getGValue() + 0.114 * this->pixels[i].getBValue();
		unsigned short maxValue = this->pixels[i].getMaxValue();
		Pixel newPixel(maxValue, grayValue, grayValue, grayValue);
		this->pixels[i] = newPixel;
	}
}

void Image::toMonochrome()
{
	// .pbm images are monochrome anyway, so there is no need to change them.
	if (this->fileExtension == ".pbm")
	{
		return;
	}
	size_t imgSize = this->width * this->height;
	for (size_t i = 0; i < imgSize; i++)
	{
		// To convert the images to monochrome(composed of only black or white pixels), I find the average value
		// of the colors that make up the pixel and check whether it is closer to white (the maximum value) 
		// or black (value 0) in the image.
		unsigned short avgValue = (this->pixels[i].getRValue() + this->pixels[i].getGValue() + this->pixels[i].getBValue()) / 3;
		unsigned short maxValue = this->pixels[i].getMaxValue();
		unsigned short monoValue = 0;
		if (avgValue >= maxValue / 2)
		{
			monoValue = maxValue;
		}

		Pixel newPixel(maxValue, monoValue, monoValue, monoValue);
		this->pixels[i] = newPixel;
	}
}

void Image::toNegative()
{
	size_t imgSize = this->width * this->height;
	for (size_t i = 0; i < imgSize; i++)
	{
		// To obtain the opposite values of the images, for each color we need to replace the value with 
		// the absolute difference between the maximum value and the current one.
		unsigned short maxValue = this->pixels[i].getMaxValue();
		unsigned short newRValue = std::abs(maxValue - this->pixels[i].getRValue());
		unsigned short newGValue = std::abs(maxValue - this->pixels[i].getGValue());
		unsigned short newBValue = std::abs(maxValue - this->pixels[i].getBValue());

		Pixel newPixel(maxValue, newRValue, newGValue, newBValue);
		this->pixels[i] = newPixel;
	}
}

// Several helper functions for the implementation of the following member functions
void initMatrix(Pixel**& matrix, const unsigned short& rowsCount, const unsigned short& colsCount)
{
	matrix = new Pixel * [rowsCount];
	for (size_t i = 0; i < rowsCount; i++)
	{
		matrix[i] = new Pixel[colsCount];
	}
}

void deleteMatrix(Pixel** matrix, const unsigned short& rowsCount)
{
	for (size_t i = 0; i < rowsCount; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}


// For rotating and achieving a mirrored image of the images, I use a temporary dynamic two-dimensional array, 
// as indexing is convenient for such manipulation.
void Image::rotateLeft()
{
	std::swap(this->height, this->width);

	Pixel** temp = nullptr;
	initMatrix(temp, this->height, this->width);

	for (size_t col = 0; col < this->width; col++)
	{
		for (int row = this->height - 1; row >= 0; row--)
		{
			temp[row][col] = this->pixels.front();
			this->pixels.erase(this->pixels.begin());
		}
	}

	for (size_t row = 0; row < this->height; row++)
	{
		for (size_t col = 0; col < this->width; col++)
		{
			this->pixels.push_back(temp[row][col]);
		}
	}

	deleteMatrix(temp, this->height);
}
void Image::rotateRight()
{
	std::swap(this->height, this->width);

	Pixel** temp = nullptr;
	initMatrix(temp, this->height, this->width);

	for (int col = this->width - 1; col >= 0; col--)
	{
		for (int row = 0; row < this->height; row++)
		{
			temp[row][col] = this->pixels.front();
			this->pixels.erase(this->pixels.begin());
		}
	}

	for (size_t row = 0; row < this->height; row++)
	{
		for (size_t col = 0; col < this->width; col++)
		{
			this->pixels.push_back(temp[row][col]);
		}
	}

	deleteMatrix(temp, this->height);
}

void Image::flipHorizontal()
{
	Pixel** temp = nullptr;
	initMatrix(temp, this->height, this->width);

	for (int row = 0; row < this->height; row++)
	{
		for (int col = this->width - 1; col >= 0; col--)
		{
			temp[row][col] = this->pixels.front();
			this->pixels.erase(this->pixels.begin());
		}
	}

	for (size_t row = 0; row < this->height; row++)
	{
		for (size_t col = 0; col < this->width; col++)
		{
			this->pixels.push_back(temp[row][col]);
		}
	}

	deleteMatrix(temp, this->height);
}
void Image::flipVertical()
{
	Pixel** temp = nullptr;
	initMatrix(temp, this->height, this->width);

	for (int row = this->height - 1; row >= 0; row--)
	{
		for (int col = 0; col < this->width; col++)
		{
			temp[row][col] = this->pixels.front();
			this->pixels.erase(this->pixels.begin());
		}
	}

	for (size_t row = 0; row < this->height; row++)
	{
		for (size_t col = 0; col < this->width; col++)
		{
			this->pixels.push_back(temp[row][col]);
		}
	}

	deleteMatrix(temp, this->height);
}

Image makeCollage(const std::string& orientation, const Image& img1, const Image& img2)
{
	// When creating a new image that is a collage of two other images, there are member variables 
	// that do not depend on the orientation of the collage. They are brought to the forefront 
	// in the member function makeCollage.
	Image collage;
	collage.filePath = img1.filePath.substr(img1.filePath.rfind('\\') + 1) + '_' + img2.filePath.substr(img2.filePath.rfind('\\') + 1);
	collage.fileExtension = img1.fileExtension;
	collage.magicNumber[0] = 'P';
	if (img1.fileExtension == ".pbm")
	{
		collage.magicNumber[1] = '1';
	}
	else if (img1.fileExtension == ".pgm")
	{
		collage.magicNumber[1] = '2';
	}
	else if (img1.fileExtension == ".ppm")
	{
		collage.magicNumber[1] = '3';
	}
	collage.magicNumber[2] = '\0';
	collage.commandsToSkip = 0;

	Pixel blackPixel;
	if (collage.fileExtension == ".pbm")
	{
		blackPixel = Pixel(1, 1, 1, 1);
	}
	else
	{
		blackPixel = Pixel(img1.pixels[0].getMaxValue(), 0, 0, 0);
	}

	if (orientation == "horizontal")
	{
		// The approach to creating a horizontal collage is as follows:
		collage.width = img1.width + img2.width;
		int heightDifference = img1.height - img2.height;

		if (heightDifference == 0)
		{
			// If the two images are of the same size, one row from the first image and one row from the second image 
			// are sequentially recorded in the pixel vector of the collage.
			collage.height = img1.height;
			for (size_t i = 0; i < img1.height; i++)
			{
				for (size_t j = 0; j < img1.width; j++)
				{
					collage.pixels.push_back(img1.pixels[i * img1.width + j]);
				}
				for (size_t j = 0; j < img2.width; j++)
				{
					collage.pixels.push_back(img2.pixels[i * img2.width + j]);
				}
			}
		}
		else
		{
			unsigned short blackRowsTop = 0, blackRowsBottom = 0;
			if (heightDifference < 0)
			{
				// If the second image has a greater height, a row of black pixels is recorded in the pixel vector 
				// of the collage, followed by a row from the second image. This is repeated (heightDifference / 2) 
				// times. Then, one row from the first image is recorded, followed by one row from the second image, 
				// and this continues until all rows from the first image are recorded. Finally, a row of black pixels 
				// is recorded again, followed by a row from the second image. This is repeated (heightDifference / 2) 
				// or (heightDifference / 2) + 1 times depending on whether heightDifference is an even or odd number.
				heightDifference = abs(heightDifference);
				blackRowsBottom = heightDifference / 2;
				blackRowsTop = heightDifference / 2;
				if (heightDifference % 2 != 0)
				{
					blackRowsBottom++;
				}

				collage.height = img2.height;
				for (size_t i = 0; i < img2.height; i++)
				{
					if (blackRowsTop > i)
					{
						for (size_t j = 0; j < img1.width; j++)
						{
							collage.pixels.push_back(blackPixel);
						}
					}
					else if (blackRowsBottom >= img2.height - i)
					{
						for (size_t j = 0; j < img1.width; j++)
						{
							collage.pixels.push_back(blackPixel);
						}
					}
					else
					{
						for (size_t j = 0; j < img1.width; j++)
						{
							collage.pixels.push_back(img1.pixels[(i - blackRowsTop) * img1.width + j]);
						}
					}
					for (size_t j = 0; j < img2.width; j++)
					{
						collage.pixels.push_back(img2.pixels[i * img2.width + j]);
					}
				}
			}
			else
			{
				// The approach when the first image has a greater height than the second is similar, just executed in reverse.
				blackRowsBottom = heightDifference / 2;
				blackRowsTop = heightDifference / 2;
				if (heightDifference % 2 != 0)
				{
					blackRowsBottom++;
				}

				collage.height = img1.height;
				for (size_t i = 0; i < img1.height; i++)
				{
					for (size_t j = 0; j < img1.width; j++)
					{
						collage.pixels.push_back(img1.pixels[i * img1.width + j]);
					}
					if (blackRowsTop > i)
					{
						for (size_t j = 0; j < img2.width; j++)
						{
							collage.pixels.push_back(blackPixel);
						}
					}
					else if (blackRowsBottom >= img1.height - i)
					{
						for (size_t j = 0; j < img2.width; j++)
						{
							collage.pixels.push_back(blackPixel);
						}
					}
					else
					{
						for (size_t j = 0; j < img2.width; j++)
						{
							collage.pixels.push_back(img2.pixels[(i - blackRowsTop) * img2.width + j]);
						}
					}
				}
			}
		}
	}
	else if (orientation == "vertical")
	{
		collage.height = img1.height + img2.height;
		int widthDifference = img1.width - img2.width;

		if (widthDifference == 0)
		{
			// For vertical collages with equal width, the approach is simple: we first record 
			// all the pixels from the first image in the pixel vector of the collage, 
			// followed by all the pixels from the second image.
			collage.width = img1.width;
			for (size_t i = 0; i < img1.pixels.size(); i++)
			{
				collage.pixels.push_back(img1.pixels[i]);
			}
			for (size_t i = 0; i < img2.pixels.size(); i++)
			{
				collage.pixels.push_back(img2.pixels[i]);
			}
		}
		else
		{
			unsigned short blackColsL = 0, blackColsR = 0;
			if (widthDifference < 0)
			{
				// If the second image has a greater width, the pixel vector of the collage records 
				// (widthDifference / 2) black pixels, followed by an entire row of pixels from the first image. 
				// The row of the collage is completed with (widthDifference / 2) or (widthDifference / 2) + 1 
				// black pixels, depending on whether widthDifference is an even or odd number. 
				// This is repeated for each row from the first image. After that, all the pixels from the second image 
				// are recorded in the pixel vector of the collage.
				widthDifference = abs(widthDifference);
				blackColsL = widthDifference / 2;
				blackColsR = widthDifference / 2;
				if (widthDifference % 2 != 0)
				{
					blackColsR++;
				}

				collage.width = img2.width;
				for (size_t i = 0; i < img1.height; i++)
				{
					for (size_t j = 0; j < blackColsL; j++)
					{
						collage.pixels.push_back(blackPixel);
					}
					for (size_t j = 0; j < img1.width; j++)
					{
						collage.pixels.push_back(img1.pixels[i * img1.width + j]);
					}
					for (size_t j = 0; j < blackColsR; j++)
					{
						collage.pixels.push_back(blackPixel);
					}
				}
				for (size_t i = 0; i < img2.pixels.size(); i++)
				{
					collage.pixels.push_back(img2.pixels[i]);
				}
			}
			else
			{
				// The approach when the first image has a greater width than the second is similar, just executed in reverse.
				blackColsL = widthDifference / 2;
				blackColsR = widthDifference / 2;
				if (widthDifference % 2 != 0)
				{
					blackColsR++;
				}

				collage.width = img1.width;
				for (size_t i = 0; i < img1.pixels.size(); i++)
				{
					collage.pixels.push_back(img1.pixels[i]);
				}
				for (size_t i = 0; i < img2.height; i++)
				{
					for (size_t j = 0; j < blackColsL; j++)
					{
						collage.pixels.push_back(blackPixel);
					}
					for (size_t j = 0; j < img2.width; j++)
					{
						collage.pixels.push_back(img2.pixels[i * img2.width + j]);
					}
					for (size_t j = 0; j < blackColsR; j++)
					{
						collage.pixels.push_back(blackPixel);
					}
				}
			}
		}
	}
	return collage;
}

void Image::crop(unsigned short xTL, unsigned short yTL, unsigned short xBR, unsigned short yBR)
{
	// It is possible to provide coordinates for the top right or bottom left point, as well as coordinates 
	// that do not form a rectangle (overlapping coordinates).
	if (yTL >= this->height)
	{
		yTL = this->height - 1;
	}
	if (yBR >= this->height)
	{
		yBR = this->height - 1;
	}
	if (xTL >= this->width)
	{
		xTL = this->width - 1;
	}
	if (xBR >= this->width)
	{
		xBR = this->width - 1;
	}

	if (xTL > xBR)
	{
		std::swap(xTL, xBR);
	}
	if (yBR > yTL)
	{
		std::swap(yBR, yTL);
	}
	if (yBR == yTL || xTL == xBR)
	{
		std::cout << "Cannot crop image " << this->filePath << " in this size. Try another values\n";
		return;
	}
	unsigned short newHeight = yTL - yBR;
	unsigned short newWidth = xBR - xTL;

	std::vector<Pixel> temp;

	int index = (this->height - yTL) * this->width + xTL;
	for (size_t i = 0; i < newHeight; i++)
	{
		for (size_t j = 0; j < newWidth; j++)
		{
			temp.push_back(this->pixels.at((index + j) + (i * this->width)));
		}
	}
	this->pixels = temp;
	this->pixels.shrink_to_fit();
	this->height = newHeight;
	this->width = newWidth;
}