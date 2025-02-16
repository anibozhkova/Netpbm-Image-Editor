#pragma once
#include "Image.h"

// Enumeration defining the available image processing commands
enum Command
{
	grayscale,     // Converts the image to grayscale
	monochrome,    // Converts the image to monochrome
	negative,      // Inverts the colors of the image
	rotateL,       // Rotates the image to the left
	rotateR,       // Rotates the image to the right
	flipH,         // Flips the image horizontally
	flipV,         // Flips the image vertically
	cropp,         // Crops the image
	collageV,      // Creates a vertical collage
	collageH,      // Creates a horizontal collage
};


// Class representing a session that manages image operations and transformations
class Session
{
private:
	static unsigned idGenerator; // Static variable to generate unique session IDs
	unsigned id;                // Unique identifier for the session
	std::vector<Image> images;       // Vector to store images associated with the session
	std::vector<unsigned short> forCollages; // Vector to store image indices for collage creation
	std::vector<unsigned short> forCollagesHistory; // History of collage image indices for undo/redo functionality
	std::vector<Command> commands;   // Vector to store commands that have been queued up for execution in the session
	std::vector<Command> undoneCommands; // Vector to store commands that can be redone
	std::vector<unsigned short> cropInfo; // Vector to store cropping information
	std::vector<unsigned short> cropInfoHistory; // History of cropping information for undo/redo functionality
	bool valid = false;         // Flag indicating whether the session is valid

public:
	// Constructors of the class:
	Session();
	Session(std::vector<std::string> filePaths);

	unsigned getId() const; // Returns the unique identifier of the session
	bool isValid() const;   // Checks if the session is valid
	void execute();			// Executes the queued commands on the images in the session
	void addCommand(const std::string&);		 // Adds a command to the session
	void addImage(const std::string& filePath); // Adds an image to the session from a specified file path
	void crop(std::vector<std::string> coordinates); // Crops the current image based on the provided coordinates
	void queueForCollage(const std::vector<std::string>& images); // Queues images for collage creation
	void undo(); // Undoes the last executed command
	void redo(); // Redoes the last undone command
	void clearUndoneCommands();			// Clears the list of undone commands
	void printInfo();					// Prints information about the session
	void printImagesNames();			// Prints the names of the images in the session
	void printPendingTransormations();  // Prints the pending transformations to be applied to the images
	void save();						// Saves the current session state	
	void saveAs(const std::string& filePath); // Saves the current session state to a specified file path

private:
	// Private helper functions
	void eraseAll(const Command&);
	void eraseFirstOccurance(const Command&);
	unsigned occurances(const Command command);
	bool containsImage(const std::string& filePath);
};