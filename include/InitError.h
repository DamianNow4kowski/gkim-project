#ifndef INIT_ERROR_H
#define INIT_ERROR_H

#include <exception>
#include <string>

class InitError : public std::exception
{
  public:
    InitError();
    InitError(const std::string &);
    virtual ~InitError();
    virtual const char *what() const noexcept;

  private:
    std::string msg;
};

#endif // !INIT_ERROR_H