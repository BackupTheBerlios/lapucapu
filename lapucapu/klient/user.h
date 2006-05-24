#ifndef USER_H
#define USER_H
#include <glibmm.h>

class user {
/**
 * Public stuff
 */
public:
	/**
	 * 
	 */
	Glib::ustring get_uid (void);
		
	
	/**
	 * 
	 */
	void set_uid (Glib::ustring s_uid);
		
	
	/**
	 * 
	 */
	Glib::ustring get_state (void);
		
	
	/**
	 * 
	 */
	void set_state (Glib::ustring s_state);
		
	
	/**
	 * 
	 */
	Glib::ustring get_opis (void);
		
	
	/**
	 * 
	 */
	void set_opis (Glib::ustring value);
		
	
/**
 * Private stuff
 */
private:
	/**
	 * Fields
	 */
	 Glib::ustring uid;
	 Glib::ustring state;
	 Glib::ustring opis;
};
#endif //USER_H

