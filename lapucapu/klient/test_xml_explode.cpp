#include <fstream>
#include <iostream>

#include <glibmm.h>
#include <string>
#include <assert.h>
#include "xml_explode.h"

using namespace std;

int main(int argc, char* argv[])
{
  std::string filepath;
  if(argc > 1 )
    filepath = argv[1]; //Allow the user to specify a different XML file to parse.
  else
  {
  	cout << "Podaj nazwe pliku do sprawdzenia:";
  	cin >> filepath;
  }
  
  cout << "Wczytuje plik: " << filepath << endl << endl;
    
  int length;
  char * buffer;
  Glib::ustring data;

  ifstream is;
  is.open (filepath.c_str(), ios::binary );

  if(is)
  	cout << "Otwarto plik" << endl;
  else
  	cout << "Klopot z otwarciem pliku" << endl;
 
//  assert(is);

//	cout << is.rdbuf();

  // get length of file:
  is.seekg (0, ios::end);
  length = is.tellg();
  is.seekg (0, ios::beg);

  // allocate memory:
  buffer = new char [length];

  // read data as a block:
  is.read (buffer,length);

  is.close();

  data.append(buffer,length);
  
  std::cout << "------ badany tekst: -----" << endl << Glib::locale_from_utf8(data) << endl << "-------------------------------" << std::endl;
  
  xml_explode test(data);

  SHOW("");
  SHOW("Testy:");
  
  SHOW("test.check_type(): ");
  Glib::ustring wynik = test.check_type();
  SHOWU(Glib::locale_from_utf8(wynik));
  
//  SHOW("++++++ Extra test from: ++++++");
//  SHOWU(test.get_uid_from ());
//  SHOW("++++++++++++++++++++++++++++++");
  
  if(wynik == "respose_login")
  {
	SHOW("-------- respose_login --------");
	  
	SHOW("Sprawdzenie czy zalogowano (test.logged()): ");
	  
	if(test.logged())
		cout << "Zalogowano" << endl;
	else
	  	cout << "Dostep zabroniony!" << endl;
	  
	SHOW("Opis logowania (test.get_login_desc()):");
	 
	SHOWU(test.get_login_desc());
	  
	SHOW("-------------------------------");
  }
  
  if(wynik == "message")
  {
  	SHOW("-----------message-------------");
  	SHOW("Wiadomosc do:");
  	uids_list lista = test.get_message_uids_to();
  	for(uids_iter iter = lista.begin(); iter != lista.end(); ++iter)
  	{
  		SHOWU(*iter);
  	} 	
  	SHOW("Tresc wiadomosci:");
  	SHOWU(test.get_message_body());
  	SHOW("-------------------------------");
  }
  
  if(wynik == "respose_add_user")
  {
  	SHOW("-----respose_add_user---------");
  	if(test.respose_add_user())
  	{
  		SHOW("Dodano usera, uid: ");
  		SHOWU(test.get_uid_from ());
  	}
  	else
  		SHOW("Niepomyslne dodanie usera!");
  	SHOW("Opis tranzakcji:");
  	SHOWU(test.get_add_user_desc());
  	SHOW("-------------------------------");
  }
  if(wynik == "respose_users_states")
  {
  	SHOW("----respose_users_states-------");
  	users_list ul = test.get_user_states();
  	users_iter ui;
  	for(ui = ul.begin(); ui != ul.end(); ++ui)
  	{
		cout << "**********" << endl;
  		cout << "User: ";
  		SHOWU((*ui).second.get_uid());
  		cout << "State: ";
  		SHOWU((*ui).second.get_state());
  		cout << "Opis: ";
  		SHOWU((*ui).second.get_opis());	
  		cout << "**********" << endl;
  	}

  	SHOW("-------------------------------");
  }
  if(wynik == "request_login")
  {
  	SHOW("-------request_login-----------");
  	SHOW("Login:");
  	SHOWU(test.get_uid_from());
  	SHOW("Passwd:");
  	SHOWU(test.get_login_passwd());
  	SHOW("-------------------------------");
  }
  
  return 0;
}
