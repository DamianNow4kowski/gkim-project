#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include <vector>
#include <unordered_set>

class InputHandler
{
protected:
	const std::string defaultOption = "open";
	std::vector<std::string> arguments;
	std::unordered_set<std::string> options;
	std::vector<std::string>::iterator arg;

public:
	InputHandler();
	InputHandler(int, char **);

	bool option(const std::string &) const;
	bool option(const char *) const;
	bool get(std::string &);
};

#endif // !INPUT_HANDLER_H
