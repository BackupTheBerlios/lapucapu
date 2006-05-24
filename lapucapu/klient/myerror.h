#ifndef MYERROR_H_
#define MYERROR_H_

#include <string>
#include <iostream>

class myerror
{
public:

	/**
	 * 
	 */
	myerror();
	
	/**
	 * 
	 */
	
	void set_last_error(std::string slast_error);

	/**
	 * 
	 */
	void critical_error(std::string why);
	
	/**
	 * 
	 */
	std::string get_last_error();
	
	/**
	 * 
	 */
	bool err_ok();
	
	/**
	 * 
	 */
	bool is_err();
	
private:
	/**
	 * 
	 */
	 std::string last_error;
	 bool iserror;
};

#endif /*MYERROR_H_*/
