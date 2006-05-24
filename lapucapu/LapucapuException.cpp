#include "LapucapuException.h"

LapucapuException::LapucapuException(const std::string& msg) : std::logic_error(msg)
{
	std::cout << "Exception::Exception()" << std::endl;
};
