#include "kontakty.h"

/**
 * Constructors/Destructors
 */
/**
 * Methods
 */
/**
 * 
 */
users_list kontakty::get_my_contacts ( ) {
	return my_contacts;
}
/**
 * 
 */
void kontakty::set_my_contacts (users_list & value ) {
	my_contacts = value;
}
/**
 * 
 */
void kontakty::add_contact (user & s_user) {
	my_contacts[s_user.get_uid()] = s_user;
}
/**
 * 
 */
void kontakty::rm_contact (user & s_user) {
	my_contacts.erase(s_user.get_uid());
}

void kontakty::rm_contact (Glib::ustring & s_uid){
	my_contacts.erase(s_uid);	
}
