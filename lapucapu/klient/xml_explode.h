#ifndef XML_EXPLODE_H_
#define XML_EXPLODE_H_

#include <libxml++.h>
#include <string>
#include <istream>
#include <glibmm.h>
#include "mytypes.h"
#include "messages.h"
#include "users_list.h"
#include "myerror.h"

class xml_explode: public myerror
{
/**
 * Public stuff
 */
public:

/**
 * Konstruktor
 */
	xml_explode(Glib::ustring sdata);
	
	/**
	 * 
	 */
	Glib::ustring check_type ();
		
	
	/**
	 * Sprawdz czy zaologowano
	 */
	bool logged ();
		
	
	/**
	 * Pobierz opis procesu logowania
	 */
	Glib::ustring get_login_desc();
		
	
	/**
	 * Pobierz tresc wiadomosci
	 */
	Glib::ustring get_message_body ();
		
	
	/**
	 * Pobierz uid "od kogo"
	 */
	Glib::ustring get_uid_from ();
		
	
	/**
	 * Poierz liste uidow do kogo
	 */
	uids_list get_message_uids_to ();
	
	std::vector<int> get_message_uids_to_int ();
	/**
	 * Sprawdzenie czy dodano uzytkownika
	 */
	 bool respose_add_user();	

	/**
	 * Pobierz opis procesu dodawania uzytkownika
	 */
	Glib::ustring get_add_user_desc();
	
	/**
	 * Pobiera liste uzytkownikow (indeksowano po uid-ie liste obiektow user)
	 */
	users_list get_user_states();
	
	
	/**
	 * dla serwera pobranie uid-a z request_login
	 */
	Glib::ustring get_login_uid();
	
	/**
	 * dla serwera pobranie hasla z request_login
	 */
	Glib::ustring get_login_passwd();
	
	 
	
private:

	xmlpp::DomParser parser;
	xmlpp::Document* xml;
	xmlpp::Element * root_node;
	
	Glib::ustring get_all_text_from_iter(xmlpp::Node::NodeList::iterator iter);
	Glib::ustring get_value_from_iter(xmlpp::Node::NodeList::iterator iter, Glib::ustring name);
};

#endif /*XML_EXPLODE_H_*/
