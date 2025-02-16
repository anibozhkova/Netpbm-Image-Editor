#include "Session.h"
#include <string>

unsigned Session::idGenerator = 0;

Session::Session() : valid(false), id(++idGenerator) { }

Session::Session(std::vector<std::string> filePaths)
{
	id = ++idGenerator;
	for (size_t i = 0; i < filePaths.size(); i++)
	{
		Image img(filePaths[i]);
		if (img.getFilePath() != "")
		{
			this->images.push_back(img);
		}

	}
	if (this->images.size() < 1)
	{
		std::cout << "Could not load any images\n";
		this->valid = false;
		return;
	}
	this->valid = true;
}

unsigned Session::getId() const {
	return this->id;
}

bool Session::isValid() const
{
	return this->valid;
}

void Session::execute()
{
	if (this->commands.size() == 0)
	{
		return;
	}
	for (size_t i = 0; i < this->images.size(); i++)
	{
		// If the number of left and right rotations is equal, the program does nothing
		if (occurances(rotateL) == occurances(rotateR) && occurances(rotateL) != 0)
		{
			eraseAll(rotateL);
			eraseAll(rotateR);
		}
		Image collage;
		unsigned timesCropped = 0;
		for (size_t j = this->images[i].getCommandsToSkip(); j < this->commands.size(); j++)
		{
			switch (this->commands[j])
			{
			case rotateL:
				this->images[i].rotateLeft();
				break;
			case rotateR:
				this->images[i].rotateRight();
				break;
			case grayscale:
				this->images[i].toGrayscale();
				break;
			case monochrome:
				this->images[i].toMonochrome();
				break;
			case negative:
				this->images[i].toNegative();
				break;
			case flipH:
				this->images[i].flipHorizontal();
				break;
			case flipV:
				this->images[i].flipVertical();
				break;
			case cropp:
				this->images[i].crop(this->cropInfo[timesCropped * 4 + 0], this->cropInfo[timesCropped * 4 + 1], this->cropInfo[timesCropped * 4 + 2], this->cropInfo[timesCropped * 4 + 3]);
				timesCropped++;
				break;
			default:
				break;
			}
		}
	}

	while (this->forCollages.size() >= 2) {
		Image collage;

		if (std::find(this->commands.begin(), this->commands.end(), collageH) != this->commands.end()) {
			collage = makeCollage("horizontal", this->images[this->forCollages[0]], this->images[this->forCollages[1]]);
		}
		else if (std::find(this->commands.begin(), this->commands.end(), collageV) != this->commands.end()) {
			collage = makeCollage("vertical", this->images[this->forCollages[0]], this->images[this->forCollages[1]]);
		}

		collage.saveImage();
		this->forCollages.erase(this->forCollages.begin(), this->forCollages.begin() + 2);
	}
	this->commands.clear();
}

void Session::addCommand(const std::string& command)
{
	if (command == "grayscale" && occurances(grayscale) == 0)
	{
		commands.push_back(grayscale);
	}
	else if (command == "monochrome" && occurances(monochrome) == 0)
	{
		commands.push_back(monochrome);
	}
	else if (command == "rotate left")
	{
		unsigned occurancesL = occurances(rotateL);
		if (occurancesL <= 1)
		{
			commands.push_back(rotateL);
		}
		else if (occurancesL == 2)
		{
			eraseAll(rotateL);
			commands.push_back(rotateR);
		}
	}
	else if (command == "rotate right")
	{
		unsigned occurancesR = occurances(rotateR);
		if (occurancesR <= 1)
		{
			commands.push_back(rotateR);
		}
		else if (occurancesR == 2)
		{
			eraseAll(rotateR);
			commands.push_back(rotateL);
		}
	}
	else if (command == "flip horizontal")
	{
		if (occurances(flipH) == 0)
		{
			commands.push_back(flipH);
		}
		else
		{
			eraseFirstOccurance(flipH);
		}
	}
	else if (command == "flip vertical")
	{
		if (occurances(flipV) == 0)
		{
			commands.push_back(flipV);
		}
		else
		{
			eraseFirstOccurance(flipV);
		}
	}
	else if (command == "negative")
	{
		commands.push_back(negative);
	}
	else if (command == "make collage horizontal")
	{
		commands.push_back(collageH);
	}
	else if (command == "make collage vertical")
	{
		commands.push_back(collageV);
	}
	else if (command == "crop")
	{
		commands.push_back(cropp);
	}
	else
	{
		std::cout << "There is no such command\n";
	}
}

void Session::addImage(const std::string& filePath)
{
	Image newImg(filePath, this->commands.size());
	this->images.push_back(newImg);
	std::cout << "Image \"" << newImg.getFilePath() << "\" added\n";
}

void Session::crop(std::vector<std::string> coordinates)
{
	for (size_t i = 0; i < 4; i++)
	{
		this->cropInfo.push_back(std::stoi(coordinates[i]));
	}
}

void Session::queueForCollage(const std::vector<std::string>& images)
{
	if (images.size() != 2)
	{
		std::cout << "Only collage of two images is possible\n";
		return;
	}
	std::string fileName1 = images[0].substr(0, images[0].length() - 4);
	std::string fileName2 = images[1].substr(0, images[1].length() - 4);
	if (!(this->containsImage(fileName1) && this->containsImage(fileName2)))
	{
		std::cout << "Both images must be part of the session\n";
		return;
	}
	std::string img1Extension = images[0].substr(images[0].length() - 3, 3);
	std::string img2Extension = images[1].substr(images[1].length() - 3, 3);
	if (img1Extension != img2Extension)
	{
		std::cout << "Cannot make a collage from different types! ("
			<< img1Extension << " and "
			<< img2Extension << ")\n";
		return;
	}
	for (size_t i = 0, j = 0; i < this->images.size() && j < 2; i++)
	{
		if (this->images[i].getFilePath() == fileName1 || this->images[i].getFilePath() == fileName2)
		{
			this->forCollages.push_back(i);
			j++;
		}
	}
}

void Session::undo()
{
	if (this->commands.size() > 0)
	{
		this->undoneCommands.push_back(this->commands[this->commands.size() - 1]);
		if (this->commands.back() == cropp)
		{
			for (size_t i = 0; i < 4; i++)
			{
				this->cropInfoHistory.insert(this->cropInfoHistory.begin(), this->cropInfo.back());
				this->cropInfo.pop_back();
			}
		}
		else if (this->commands.back() == collageH || this->commands.back() == collageV)
		{
			for (size_t i = 0; i < 2; i++)
			{
				this->forCollagesHistory.insert(this->forCollagesHistory.begin(), this->forCollages.back());
				this->forCollages.pop_back();
			}
		}
		this->commands.pop_back();
	}
	else
	{
		std::cout << "There is no commands to be undone\n";
	}
}

void Session::redo()
{
	if (this->undoneCommands.size() > 0)
	{
		this->commands.push_back(this->undoneCommands[this->undoneCommands.size() - 1]);
		if (this->undoneCommands.back() == cropp)
		{
			for (size_t i = 0; i < 4; i++)
			{
				this->cropInfo.push_back(this->cropInfoHistory[i]);
			}
			this->cropInfoHistory.erase(this->cropInfoHistory.begin(), this->cropInfoHistory.size() >= 4 ? this->cropInfoHistory.begin() + 4 : this->cropInfoHistory.end());
		}
		else if (this->undoneCommands.back() == collageH || this->commands.back() == collageV)
		{
			for (size_t i = 0; i < 2; i++)
			{
				this->forCollages.push_back(this->forCollagesHistory[i]);
			}
			this->forCollagesHistory.erase(this->forCollagesHistory.begin(), this->forCollagesHistory.size() >= 2 ? this->forCollagesHistory.begin() + 2 : this->forCollagesHistory.end());
		}
		this->undoneCommands.pop_back();
	}
	else
	{
		std::cout << "No commands to be redone\n";
	}
}

void Session::clearUndoneCommands()
{
	// For after recieving a new command after redo
	this->undoneCommands.clear();
}

void Session::printInfo()
{
	std::cout << "Session ID: " << id << "\nImages in this session:\n";
	printImagesNames();
	std::cout << "Pending transformations:\n";
	printPendingTransormations();
}

void Session::printImagesNames()
{
	for (size_t i = 0; i < images.size(); i++)
	{
		std::cout << images[i].getFilePath() << " ";
	}
	std::cout << "\n";
}

void Session::printPendingTransormations()
{
	for (size_t i = 0; i < this->commands.size(); i++)
	{
		switch (this->commands[i])
		{
		case grayscale:
			std::cout << "grayscale "; break;
		case monochrome:
			std::cout << "monochrome "; break;
		case negative:
			std::cout << "negative "; break;
		case rotateL:
			std::cout << "rotate left "; break;
		case rotateR:
			std::cout << "rotate right "; break;
		case collageH:
			std::cout << "collage horizontal "; break;
		case collageV:
			std::cout << "collage vertical "; break;
		}
	}
	std::cout << "\n";
}

void Session::saveAs(const std::string& filePath)
{
	if (this->images.size() > 0)
	{
		Image newImg(this->images[0]);
		newImg.setFilePath(filePath);
		newImg.saveImage();
	}
	else
	{
		std::cout << "There are no images in this session\n";
	}
}

void Session::save()
{
	for (size_t i = 0; i < this->images.size(); i++)
	{
		this->images[i].saveImage();
	}
}

unsigned Session::occurances(const Command command)
{
	unsigned count = 0;
	for (size_t i = 0; i < this->commands.size(); i++)
	{
		if (this->commands[i] == command)
		{
			count++;
		}
	}
	return count;
}

void Session::eraseAll(const Command& command)
{
	for (size_t i = 0; i < this->commands.size(); i++)
	{
		if (this->commands[i] == command)
		{
			this->commands.erase(this->commands.begin() + i);
			i--;
		}
	}
}

void Session::eraseFirstOccurance(const Command& command)
{
	for (size_t i = 0; i < this->commands.size(); i++)
	{
		if (this->commands[i] == command)
		{
			this->commands.erase(this->commands.begin() + i);
			break;
		}
	}
}

bool Session::containsImage(const std::string& filePath)
{
	for (size_t i = 0; i < this->images.size(); i++)
	{
		if (this->images[i].getFilePath() == filePath)
		{
			return true;
		}
	}
	return false;
}