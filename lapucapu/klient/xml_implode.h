#ifndef XML_IMPLODE_H
#define XML_IMPLODE_H

#include <libxml++.h>
#include <string>
#include <istream>
#include <glibmm.h>
#include "mytypes.h"
#include "messages.h"
#include "users_list.h"
#include "myerror.h"

class xml_implode: public myerror {
/**
 * Public stuff
 */
public:
	/**
	 * Konstruktor - podaj tylko typ xml-a a taki zostanie zbudowany
	 */
	xml_implode(Glib::ustring stype); 
	
	/**
	 * 
	 */
	Glib::ustring get_data ();
	
	//---------------------------------------------
	/**
	 *  message.xml
	 */
	bool set_message_from (Glib::ustring suid);
	
	/**
	 * 
	 */
	bool set_message_to (Glib::ustring suid);
		
	/**
	 * 
	 */
	bool set_message_body (Glib::ustring stext);

	//---------------------------------------------
	
	/**
	 * request_login.xml
	 */
	bool set_login_data (Glib::ustring suid, Glib::ustring spasswd);

	/**
	 * request_add_user.xml
	 */
	bool set_new_user_passwd (Glib::ustring spasswd);
	
	/**
	 * request_change_status.xml
	 */
	bool set_status (Glib::ustring suid, Glib::ustring sstatus, Glib::ustring sopis);
	
	/**
	 * request_users_list.xml
	 */
	bool set_uid_request_users_list (Glib::ustring suid);
	
	/**
	 * request_users_states.xml
	 */
	bool set_uid_request_users_states (Glib::ustring suid);
	
private:

	/**
	 * 
	 */
	xmlpp::DomParser parser;
	xmlpp::Document* xml;
	xmlpp::Element * root_node;
	Glib::ustring data;
	Glib::ustring type;
		
};
#endif //XML_IMPLODE_H
