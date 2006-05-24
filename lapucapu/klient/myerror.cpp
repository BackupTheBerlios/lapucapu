#include "myerror.h"

/**
 * 
 */	
myerror::myerror()
{
	iserror = false;	
}

/**
 * 
 */	
void myerror::set_last_error(std::string slast_error) {
	last_error = slast_error;
	iserror = true;	
}

/**
 * 
 */
std::string myerror::get_last_error() {
	if(iserror) {
		iserror = false;
		std::string temp = last_error;
		last_error = "";
		iserror = false;
		return temp;
	}
	else 
		return 0;
}

/**
 *  to jest zrobione tak dla jaj :P
 *  nie wiedzia?em z czego bedzie wygodniej korzystac :P
 */
bool myerror::err_ok() {
	
	if(iserror)
		return false;
	else
		return true;
}

/**
 * 
 */
bool myerror::is_err() {
	
	return iserror;
}

/**
 * 
 */
void myerror::critical_error(std::string why) {
	std::cout << std::endl << "!!critical error!!" << std::endl << why << std::endl;
	exit(-2);
}
