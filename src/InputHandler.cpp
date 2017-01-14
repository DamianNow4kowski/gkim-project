#include "InputHandler.h"
#include "CText.h"

#include <iostream>
#include <string>


InputHandler::InputHandler(int argc, char ** argv)
	:EMPTY_OPTION("input")
{

#ifdef _DEBUG
	std::cout << " -> [InputHandler]: Parsing input for executable '" << CText(argv[0], CText::Color::GREEN) << '\'' << std::endl;
#endif // DEBUG

	std::string option = EMPTY_OPTION;
	std::vector<std::string> arguments;

	size_t i = 1;
	while (true)
	{
		// add arguments
		if (i < argc && argv[i][0] != '-')
		{
			arguments.push_back(argv[i]);
		}

		else
		{
			// try insert into map
			bool emplaced;
			std::unordered_map <std::string, std::vector<std::string>>::iterator it;
			std::tie(it, emplaced) = option_arguments.emplace(option, arguments);

			// when key exists update it
			if (!emplaced)
			{
				for (auto &a : arguments)
					(*it).second.push_back(a);
			}

			if (i == argc)
				break;

			// clean arguments for new option
			arguments.clear();
			option = argv[i];
			option.erase(0, 1);
		}
		++i;
	}
}


bool InputHandler::isset(const std::string & option) const
{
	return option_arguments.count(option) == 1;
}

bool InputHandler::isset(const char * option) const
{
	return isset(std::string(option));
}

std::vector<std::string> InputHandler::get(const std::string &argument)
{
	auto search = option_arguments.find(argument);
	if (!used.count(argument) && search != option_arguments.end())
	{
		used.emplace(argument);
		return std::move((*search).second);
	}
	return std::vector<std::string>();
}

std::vector<std::string> InputHandler::get(const char * option)
{
	return get(std::string(option));
}

bool InputHandler::empty() const
{
	return option_arguments.size() == used.size();
}

void InputHandler::print(std::ostream & o)
{
	o << "\n";
	for (const auto &vec : option_arguments)
	{
		o << "     -> " << CText(vec.first, CText::Color::CYAN) << std::endl;
		for (const auto &s : vec.second)
		{
			o << "        - " << s << std::endl;
		}
	}
	o << std::endl;
}
