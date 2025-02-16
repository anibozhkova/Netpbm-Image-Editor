#include "Pixel.h"

Pixel::Pixel() : maxValue(0), red(0), green(0), blue(0) { }

Pixel::Pixel(const unsigned short& maxValue, const unsigned short& red, const unsigned short& green, const unsigned short& blue)
{
	setMaxValue(maxValue);
	setValues(red, green, blue);
}

Pixel::~Pixel() { }

Pixel& Pixel::operator=(const Pixel& other)
{
	if (*this != other)
	{
		this->maxValue = other.maxValue;
		this->red = other.red;
		this->green = other.green;
		this->blue = other.blue;
	}
	return *this;
}

bool Pixel::operator==(const Pixel& other)
{
	return (this->maxValue == other.maxValue && this->red == other.red && this->green == other.green && this->blue == other.blue);
}

bool Pixel::operator!=(const Pixel& other)
{
	return !(*this == other);
}

void Pixel::setMaxValue(const unsigned short& value)
{
	if (value <= 255 && value >= 1)
	{
		this->maxValue = value;
	}
	else
	{
		std::cout << "A pixel's maximum value can range from 1 to 255\n";
	}
}

void Pixel::setValues(const unsigned short& red, const unsigned short& green, const unsigned short& blue)
{
	if (red < 0 || green < 0 || blue < 0)
	{
		std::cout << "Incorrect RGB values.\n";
	}
	if (red <= this->maxValue && green <= this->maxValue && blue <= this->maxValue)
	{
		this->red = red;
		this->green = green;
		this->blue = blue;
	}
	else
	{
		std::cout << "Incorrect RGB values.\n";
	}
}

short Pixel::getMaxValue() const
{
	return this->maxValue;
}

short Pixel::getRValue() const
{
	return this->red;
}

short Pixel::getGValue() const
{
	return this->green;
}

short Pixel::getBValue() const
{
	return this->blue;
}