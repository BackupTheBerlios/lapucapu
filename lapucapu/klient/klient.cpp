#include <glibmm.h>
#include "post_office.h"
#include "klient.h"

/**
 * Constructors/Destructors
 */
klient::klient (post_office* s_connection)
{
	connection = s_connection;
}
/**
 * Methods
 */
 
/**
 * 
 */
bool klient::login (int uid, Glib::ustring passwd) 
{
	//wyslanie do serwera zadania zalogowania uzytkownika do systemu
	return true; // je?li logowanie poprawne
}

/**
 * 
 */
bool klient::logout()
 {
 	//todo: wyslanie zadania wylogowania uzytkownika
 	 
 	return true; 
 }
 
/**
 * 
 */
int klient::get_uid ( ) {
	return uid;
}
/**
 * 
 */
bool klient::new_user(Glib::ustring s_passwd) {
	passwd = s_passwd;
	//todo: wyslanie zadania dodania uzytkownika do systemu
}
/**
 * 
 */
Glib::ustring klient::get_passwd ( ) {
	return passwd;
}
/**
 * 
 */
void klient::set_passwd (Glib::ustring value ) {
	passwd = value;
	//todo: wys?anie nowego hasla do servera
}
