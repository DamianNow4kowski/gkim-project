#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

class InputHandler
{
protected:

	const std::string EMPTY_OPTION;

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

	bool isset(const std::string &option) const;
	bool isset(const char *option) const;

	std::vector<std::string> get(const std::string &option);
	std::vector<std::string> get(const char *option);

	bool empty() const;

	void print(std::ostream &o = std::cout);
};

#endif // !INPUT_HANDLER_H
