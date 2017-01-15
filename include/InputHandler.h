#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <regex>
#include <tuple>

class InputHandler
{
protected:
	
	const std::string DEFAULT_OPTION;
	const std::regex FILE_EXT;

	std::unordered_set<std::string> used;
	std::unordered_map<std::string, std::vector<std::string>> option_arguments;

	/*std::vector<std::string> arguments;
	std::unordered_set<std::string> options;
	std::vector<std::string>::iterator arg;*/

public:

	InputHandler(int argc, char ** argv);
	InputHandler(const InputHandler &) = default;
	InputHandler(InputHandler &&) = default;
	InputHandler& operator=(const InputHandler &) = default;
	InputHandler& operator=(InputHandler &&) = default;
	InputHandler() = default;

	bool isset(const std::vector<std::string> &options) const;
	bool isset(const char *option) const;

	std::vector<std::string> get(const std::string &options);
	std::vector<std::string> get(const char *option);

	bool empty() const;

	/**
	 * Checks wheter 'filepath' leads to file with given 'extension'
	 * @param std::string full path to file
	 * @param char[] extension to match
	 * @return pair<bool, string>
	 * - true when file is matched with one of extensions, returns full filename (with extension)
	 * - false otherwise, return only file extension or "" if there isn't one
	 */
	std::pair<bool, std::string> match_extension(const std::string &filepath, const char * extension) const;

	/**
	 * Checks wheter 'filepath' leads to file compatible with one of 'extensions'
	 * @param std::string full path to file
	 * @param std::vector<(std::string | char[])> extensions to match
	 * @return tuple of:
	 * - bool true when matched | false otherwise
	 * - string filename with extension (null when not a proper filepath)
	 * - string extension (null when not a proper filepath)
	 */
	std::tuple<bool, std::string, std::string> match_extensions(const std::string &filepath, const std::vector<std::string> &extensions) const;
	
	void print(std::ostream &o = std::cout);
};

#endif // !INPUT_HANDLER_H
