#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include <vector>
#include <unordered_set>

class InputHandler
{
protected:
	const bool debug;
	const std::string defaultOption = "open";
	std::vector<std::string> arguments;
	std::unordered_set<std::string> options;
	std::vector<std::string>::iterator arg;

public:
	InputHandler(bool = false);
	InputHandler(int, char **, bool = false);

	bool option(const std::string &) const;
	bool option(const char *) const;
	bool get(std::string &);

	~InputHandler();
};

#endif // !INPUT_HANDLER_H
