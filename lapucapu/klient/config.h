#ifndef CONFIG_H
#define CONFIG_H

class config {
/**
 * Public stuff
 */
public:
	/**
	 * 
	 */
	 config(std::string s_file_name);
	 /**
	  * 
	  */
	 ~config();
	/**
	 * 
	 */
	int get_uid();
		
	
	/**
	 * 
	 */
	std::string get_passwd();
		
	
	/**
	 * 
	 */
	bool is_configured();
	
	/**
	 * 
	 */
	std::string get_file_name ( );
		
	
	/**
	 * 
	 */
	void set_file_name (std::string s_file_name);
		
	
/**
 * Private stuff
 */
private:
	/**
	 * Fields
	 */
	std::string file_name;
	bool configured;
	/**
	 * 
	 */
	
};
#endif //CONFIG_H

