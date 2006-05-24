#ifndef KLIENT_H
#define KLIENT_H

#include "messages.h"
#include "mytypes.h"

class klient {
/**
 * Public stuff
 */
public:

/**
 * Constructors/Destructors
 */
	klient(post_office* s_connection);
	
	~klient();
	
	/**
	 * 
	 */
	bool login(int uid, Glib::ustring passwd);

	/**
	 * 
	 */
	 bool logout();
	
	/**
	 * 
	 */
	int get_uid ();
		
	
	/**
	 * 
	 */
	void set_uid (int value );
		
	
	/**
	 * 
	 */
	Glib::ustring get_passwd ( );
	
	/**
	 * 
	 */
	 
	void set_passwd (Glib::ustring value );	
	
	/**
	 * 
	 */
	 
	bool new_user(Glib::ustring passwd);
	
/**
 * Private stuff
 */
private:
	/**
	 * Fields
	 */
	int uid;
	Glib::ustring passwd;
	post_office* connection;

};
#endif //KLIENT_H

