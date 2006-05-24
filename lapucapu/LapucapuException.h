#ifndef LAPUCAPUEXCEPTION_H_
#define LAPUCAPUEXCEPTION_H_

#include <iostream>
#include <stdexcept>

class LapucapuException : public std::logic_error
{
public:
	LapucapuException(const std::string& msg="");
};

#endif /*LAPUCAPUEXCEPTION_H_*/
