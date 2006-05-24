#ifndef OBSLUGAKLIENTA_H_
#define OBSLUGAKLIENTA_H_

#include <deque>
#include <iterator>
#include <map>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <libxml++/libxml++.h>
#include "Package.h"
#include "OnlineUsersList.h"
#include "User.h"
#include "BazaDanych.h"
//#include "klient/mytypes.h"

class ObslugaKlienta : public BazaDanych
{
	int user_id;
	User* user;
	OnlineUsersList& users_list;
	Package data_to_pkg(const Glib::ustring& data);
	int send_pkg(Package& pkg);
	
	GMutex *wyslij;
	int cs;
	int suid; // uid uzytkownika na serwerze
	std::string authid; // uid jakim posluguje sie uzytkownik podczas logowania
	std::string authpass; // haslo :)
	std::string tabela;
	
	
public:
	ObslugaKlienta(OnlineUsersList& oul, int usr_id, int socket_desc);
	~ObslugaKlienta();

	int send_str(const Glib::ustring& data);
	Glib::ustring receive_str();
	
	int my_send(gpointer dane, int dlugosc_danych);
	int my_recv(gpointer dane, int dlugosc_danych);
	void sprzatacz();
	
	/*
	 * nie wiem jeszcze jak rozwiazac sprawe sprzatacza
	 * ale chyba po prostu wy wowolasz sprzatacza,
	 * ja wywolam terminate i kazdy jest zadowolony
	 * na koniec juz w watku sie tylkoe zrobi delete na obiekcie i pasuje
	 */
	 
	void terminate();
};

#endif /*OBSLUGAKLIENTA_H_*/
