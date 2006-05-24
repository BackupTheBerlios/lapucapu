#ifndef KONTAKTY_H
#define KONTAKTY_H
#include <string>
#include "users_list.h"

class kontakty {
/**
 * Public stuff
 */
public:

	/**
	 * 
	 */
	void add_contact (user & s_user);
		
	
	/**
	 * 
	 */
	void rm_contact (user & s_user);
	
	/**
	 * 
	 */
	void rm_contact (Glib::ustring & s_uid);
		
	/**
	 * 
	 */
	users_list get_my_contacts ( );
		
	
	/**
	 * 
	 */
	void set_my_contacts (users_list & value);
/**
 * Private stuff
 */
private:
	/**
	 * Fields
	 */
	 users_list my_contacts;
	/**
	 * 
	 */		
};
#endif //KONTAKTY_H

