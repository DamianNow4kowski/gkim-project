#ifndef INIT_ERROR_H
#define INIT_ERROR_H

#include <exception>
#include <SDL.h>
#include <string>

class InitError : public std::exception 
{
public:
	InitError();
	InitError(const std::string&);
	virtual ~InitError();
	virtual const char* what() const;

private:
	std::string msg;
};

#endif // !INIT_ERROR_H