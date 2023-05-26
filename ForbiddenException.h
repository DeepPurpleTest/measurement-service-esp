#ifndef FORBIDDEN_EXCEPTION_H
#define FORBIDDEN_EXCEPTION_H
#include <exception>

class ForbiddenException : public std::exception {
public:
    const char* what() const throw() {
        return "403 Forbidden Exception";
    }
};

#endif