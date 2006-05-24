#include "user.h"

/**
 * Constructors/Destructors
 */
/**
 * Methods
 */
/**
 * 
 */
Glib::ustring user::get_uid (void) 
{
	return uid;
};
/**
 * 
 */
void user::set_uid (Glib::ustring s_uid)
{
	uid = s_uid;
};
/**
 * 
 */
Glib::ustring user::get_state (void) 
{
	return state;
};
/**
 * 
 */
void user::set_state (Glib::ustring s_state) 
{
	state = s_state;
};
/**
 * 
 */
Glib::ustring user::get_opis (void)
{
	return opis;
};
/**
 * 
 */
void user::set_opis (Glib::ustring value) 
{
	opis = value;
};

