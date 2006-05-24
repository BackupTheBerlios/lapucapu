/**
 * Klasa users_data, przechowuje "wieksze" informacje o kontaktach
 * ktore bede zapisane do pliku gdyz nie potrzebujemy ich ciagle w pamieci
 * 
 */

#ifndef USERS_DATA_H_
#define USERS_DATA_H_
#include "glibmm.h"

class users_data
{
public:
	Glib::ustring get_user_nick(Glib::ustring uid);
	Glib::ustring get_user_name(Glib::ustring uid);
	Glib::ustring get_user_surname(Glib::ustring uid);		
private:
		
};

#endif /*USERS_DATA_H_*/
