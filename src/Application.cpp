#include "SDL_Local.h"
#include "RGB12.h"
#include "BMP.h"
#include "InputHandler.h"
#include "CText.h"
#include "RuntimeError.h"

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

int main(int argc, char *argv[])
{
	InputHandler cli(argc, argv);

	if (cli.isset("debug"))
	{
		cli.print();
	}

	const std::vector<std::string> SUPPORTED_EXTS = { "bmp", "rgb12" };

	// Parse files on input
	// 0 => filepath
	// 1 => filename
	// 2 => extension
	std::vector<std::tuple<std::string, std::string, std::string>> parsedFiles;
	for (auto &f : cli.get("input"))
	{

#ifdef _WIN32
		// normalize path separator
		for (auto &c : f)
		{
			if (c == '\\')
				c = '/';
		}
#endif
		auto res = cli.match_extensions(f, SUPPORTED_EXTS);
		if (std::get<0>(res))
		{
			parsedFiles.push_back(std::make_tuple(f, std::get<1>(res), std::get<2>(res)));
		}
		else std::cerr << " !!! Unsupported extension: " << CText((std::get<2>(res).empty() ? " - no extension -" : std::get<2>(res))) << std::endl;
	}

	if (!parsedFiles.empty())
	{
		// Initialize SDL
		try
		{
			SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		}
		catch (const RuntimeError &err)
		{
			std::cerr << " !!! Error while initializing SDL:  " << CText(err.what()) << std::endl;
			return 1;
		}

		for (auto &file : parsedFiles)
		{
			std::string path, name, ext;
			std::tie(path, name, ext) = file;

			RGB12 input;

			// Load input file proper way
			if (ext == "bmp")
			{
				BMP bmp_input;
				bmp_input.load(path);
				input = std::move(bmp_input);
			}
			else
			{
				input.load(path);
			}

			// Load Image
			if (!input.image.empty())
			{
				// Convert to gray scale if needed
				if (cli.isset({ "gs" , "gray", "grayscale" }))
					input.toGrayScale();

				if (cli.isset({ "v", "view" }))
					input.preview();
			}
		}
	}
	else
	{
		std::cerr << "There was no input provided or it was invaild." << std::endl;
	}

	// Return sucess
	return EXIT_SUCCESS;
}
