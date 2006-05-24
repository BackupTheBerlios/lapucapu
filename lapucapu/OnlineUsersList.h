#ifndef ONLINEUSERSLIST_H_
#define ONLINEUSERSLIST_H_

#include <map>
#include <iostream>
#include <glib.h>
#include <glibmm.h>
#include <mysql++.h>
#include "User.h"
#include "BazaDanych.h"
#include "LapucapuException.h"
#include "klient/xml_explode.h"

class OnlineUsersList : public BazaDanych
{
	std::map<int, User> users_list;
	std::map<int, User>::iterator it;
	
	signed int users_count;
	const static int ServicePort=4096;
	const static int MaxClients=100;
	GMutex* mutex;
	GMutex* mutex_login;
	
public:
	OnlineUsersList();	
	~OnlineUsersList();

	User* operator[](int user_id);
	
	void add_user(User& user);	
	void remove_user(int user_id);
	
	
	//int login(const int& login, const std::string& password, mysqlpp::Connection& polaczenie);
	//int login(const int& login, const std::string& password);
	int login(const Glib::ustring& xml_data, mysqlpp::Connection& polaczenie);
	int logout(int user_id);
	bool is_aviable();	//o ile dobrze zrozumialem intencje, kontroliujemy ile chcemy miec userow
	int get_client_no() const {return users_count;}
	int get_service_port()const {return ServicePort;}
	int get_max_clients()const{return MaxClients;}
};

#endif /*ONLINEUSERSLIST_H_*/
