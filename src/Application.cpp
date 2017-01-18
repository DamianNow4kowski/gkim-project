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
#include <regex>

#ifdef _WIN32
void normalizePathSeparator(std::string &path)
{
	for (auto &c : path)
	{
		if (c == '\\')
			c = '/';
	}
}
#endif

void replaceAll(std::string & str, const std::string & find, const std::string & replace)
{
	size_t findSize = find.length();
	size_t replaceSize = replace.length();

	for (size_t i = 0; (i = str.find(find, i)) != std::string::npos; i += replaceSize)
	{
		str.replace(i, findSize, replace);
	}
}


std::string proccessOutputPattern(std::string output, size_t id, const std::string &path, const std::string &name, const std::string &ext)
{
	replaceAll(output, "%input.path%", name);
	replaceAll(output, "%input.name%", name);
	replaceAll(output, "%input.ext%", ext);
	replaceAll(output, "%input.id%", std::to_string(id));
	return std::move(output);
}


int main(int argc, char *argv[])
{
	InputHandler cli(argc, argv);

	if (cli.isset({ "v", "version" }))
	{
		std::cout << "Github: https://github.com/k911/gkim-project" << std::endl
			<<  CText("Copyright (C) 2017 v1.0.0-RC1", CText::Color::GREEN);
		return EXIT_SUCCESS;
	}

	if (cli.isset({ "a" , "authors" }))
	{
		std::cout << " - marmal95 | https://github.com/marmal95 \n - Trub4dur | https://github.com/Trub4dur \n - k911 | https://github.com/k911 \n - pan-marek | https://github.com/pan-marek";

		return EXIT_SUCCESS;
	}

	if (cli.isset({ "h", "help" }))
	{
		std::cout << "\n Usage:\n" << std::endl
			<< "\t[-input] {...input file(s)} [optional parameters]\tnormal usage" << std::endl
			<< "\t-a | -authors\t\t\t\t\t\tshows authors information" << std::endl
			<< "\t-v | -version\t\t\t\t\t\tshows program version" << std::endl
			<< "\t-h | -help\t\t\t\t\t\tshow this informations" << std::endl
			<< "\n Optional parameters:\n" << std::endl
			<< "\t-output \"filepath\"\toutput file path pattern, can use following variables:\n" << std::endl
			<< "\t\t\t\t\t" << CText("%input.path%", CText::Color::YELLOW) << "\t[unix] path of file (if not null, ends with '/')" << std::endl
			<< "\t\t\t\t\t" << CText("%input.name%", CText::Color::YELLOW) << "\tname of input file (without extension)" << std::endl
			<< "\t\t\t\t\t" << CText("%input.ext%", CText::Color::YELLOW) << "\textension of input file (without dot)" << std::endl
			<< "\t\t\t\t\t" << CText("%input.id%", CText::Color::YELLOW) << "\tposition in input files set\n" << std::endl
			<< "\t-s | -show\t\tshows output file" << std::endl
			<< "\t-g | -gs | -gray\tconverts image to grayscale (even if it is already in grayscale!)" << std::endl
			<< "\t-huffman | -lz77\tuses choosen algorithm instead of default one (BitDensity)" << std::endl;

		return EXIT_SUCCESS;
	}

	const std::regex save_bmp(R"(.*\.bmp$)");
	const std::vector<std::string> SUPPORTED_EXTS = { "bmp", "rgb12" };

	// Parse files on input
	// 0 => full filepath
	// 0 => path only (can be null, if not ends on '/')
	// 1 => filename (without extension)
	// 2 => extension (without dot)
	std::vector<std::tuple<std::string, std::string, std::string, std::string>> parsedFiles;
	for (auto &f : cli.get("input"))
	{

#ifdef _WIN32
		normalizePathSeparator(f);
#endif
		auto res = cli.match_extensions(f, SUPPORTED_EXTS);

		// If extension matches
		if (std::get<0>(res))
		{
			parsedFiles.push_back(std::make_tuple(f, std::get<1>(res), std::get<2>(res), std::get<3>(res)));
		}

		else std::cerr << '[' << CText("Unsupported extension") << "]: " << (std::get<3>(res).empty() ? CText("(no extension)", CText::Color::YELLOW) : CText(std::get<3>(res), CText::Color::WHITE)) << std::endl;
	}

	

	// If there is any output start preparing dependencies
	if (!parsedFiles.empty())
	{

		// Initialize SDL
		try
		{
			SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		}
		catch (const RuntimeError &err)
		{
			std::cerr << '[' << CText("SDL Error") << "]: " << err.what() << std::endl;
			return 1;
		}

		// Get output pattern if exists
		std::vector<std::string> outputPatterns = cli.get("output");
		bool isOutput = !outputPatterns.empty();

		if (isOutput)
		{
			for (auto &out : outputPatterns)
			{
#ifdef _WIN32
				normalizePathSeparator(out);
#endif
			}
		}
		
		// Change save algorithm if set
		RGB12::Algorithm alg = RGB12::Algorithm::BitDensity;
		if (cli.isset("huffman"))
			alg = RGB12::Algorithm::Huffman;
		else if (cli.isset("lz77"))
			alg = RGB12::Algorithm::LZ77;

		// Start processing files
		size_t id = 0;
		for (auto &file : parsedFiles)
		{
			std::string fullpath, path, name, ext;
			std::tie(fullpath, path, name, ext) = file;

			RGB12 input;

			// Load input file proper way
			if (ext == "bmp")
			{
				BMP bmp_input;
				bmp_input.load(fullpath);
				input = std::move(bmp_input);
			}
			else
			{
				input.load(fullpath);
			}

			// Load Image
			if (!input.image.empty())
			{
				input.algorithm = alg;

				// Convert to gray scale if needed
				if (cli.isset({ "g" , "gs", "gray" }))
					input.toGrayScale();

				// Save with chosen algoirthm if any output set
				if (isOutput)
				{
					std::string outputFile = proccessOutputPattern(outputPatterns[0], id, path, name, ext);
					if (std::regex_match(outputFile, save_bmp))
					{
						BMP f = std::move(input);
						f.save(outputFile);
					}
					else
					{
						input.save(outputFile);
					}

					// cout saved file to enable possibility for futher use in console
					std::cout << outputFile << std::endl;
				}

				// Show the output if wanted
				if (cli.isset({ "s", "show" }))
					input.preview();

				++id;
			}
		}
	}
	else
	{
		std::cout << '[' << CText("Warning", CText::Color::YELLOW) << "]: There was no input provided or it was invaild." << std::endl;
		std::cout << "If you don't know how to use type -h or -help." << std::endl;
	}

	// Return sucess
	return EXIT_SUCCESS;
}
