#pragma once

#include <fstream>
#include <string>

#include "ImageSurface.h"

// [I]mage [V]iew and [I]nput/[O]utput [O]perations [H]andler
class ImageHandler
{
private:

	/// Utility functions

	/**
	 * Verifies extension of file
	 * @param string with filename 
	 * @param string containg verified extension WITH dot (!important)
	 * @return true if extension matches | false otherwise
	 */
	bool verifyExtension(const std::string &, const std::string &) const;


protected:

	virtual void store(const std::string &, const ImageSurface &) const = 0;
	virtual ImageSurface recover(const std::string &) = 0;

	/// Utility functions for derieved class

	/**
	 * Opens input file stream
	 * @throws RuntimeError
	 */
	void openStream(const std::string &, std::ifstream &) const;

	/**
	 * Opens output file stream
	 * @throws RuntimeError
	 */
	void openStream(const std::string &, std::ofstream &) const;


	/**
	 * Helper protected constructor for directly attaching given SDL_Surface to ImageSurface 
	 * Remarks: should be used only if SDL_Surface was copied before [ex. in derieved class] to avoid copying it second time
	 */
	ImageHandler(SDL_Surface*);

public:

	// Image Container
	ImageSurface image;

	// Default constructor
	ImageHandler();

	// Copy SDL_Surface constructor
	ImageHandler(const SDL_Surface*); // TODO: investigate if needed

	// Copy ImageSurface constructor
	ImageHandler(const ImageSurface &);

	// Copy constuctor
	ImageHandler(const ImageHandler &);

	// Move constructor
	ImageHandler(ImageHandler &&);

	// Copy assigment operator
	ImageHandler & operator=(const ImageHandler &);

	// Move assigment operator
	ImageHandler & operator=(ImageHandler &&);

	// virtual destructor due to abstraction of this class
	virtual ~ImageHandler();

	/// Public interface methods

	// Render ImageSurface view and show it on the screen 
	void preview(bool = false);

	// Saving ImageSurface to file handler
	void save(std::string &) const;
	void save(const char*) const;

	// Loading data from file handler to init ImageSurface
	void load(const std::string &);
	void load(const char*);

	// @return supported extension to save to/load from
	virtual std::string extension() const = 0;

};

