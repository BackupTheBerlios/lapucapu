#include <fstream>
#include <iostream>

#include <glibmm.h>
#include <string>
#include <assert.h>
#include "xml_implode.h"

using namespace std;

int kmain(int argc, char* argv[])
{
	SHOW("Podaj typ xml-a:");
	string data, data1, data2;
	cin >> data;
	if(data == "message")
	{
		xml_implode test(Glib::locale_to_utf8(data));
		SHOW("Podaj uid from:");
		cin >> data;
		assert(test.set_message_from(Glib::locale_to_utf8(data)));
		SHOW("Podaj uid to:");
		cin >> data;	
		assert(test.set_message_to(Glib::locale_to_utf8(data)));	
		SHOW("Podaj tresc wiadomosci:");
		cin >> data;
		SHOW(data);
		assert(test.set_message_body(Glib::locale_to_utf8(data)));
		SHOW("------ xmldata ------");
		SHOWU(test.get_data());
		SHOW("----- end xmldata -----");
		return 0;
	}
	if(data == "request_login")
	{
		xml_implode test(Glib::locale_to_utf8(data));	
		SHOW("Podaj uid i passwd:");
		cin >> data >> data1;
		assert(test.set_login_data(Glib::locale_to_utf8(data), Glib::locale_to_utf8(data1)));
		SHOW("------ xmldata ------");
		SHOWU(test.get_data());
		SHOW("----- end xmldata -----");
		return 0;
	}
	if(data == "request_add_user")
	{
		xml_implode test(Glib::locale_to_utf8(data));	
		SHOW("Podaj passwd:");
		cin >> data;
		assert(test.set_new_user_passwd(Glib::locale_to_utf8(data)));
		SHOW("------ xmldata ------");
		SHOWU(test.get_data());
		SHOW("----- end xmldata -----");
		return 0;
	}
	if(data == "request_change_status")
	{
		xml_implode test(Glib::locale_to_utf8(data));	
		SHOW("Podaj uid, status i opis:");
		cin >> data >> data1 >> data2;
		assert(test.set_status(Glib::locale_to_utf8(data), Glib::locale_to_utf8(data1), Glib::locale_to_utf8(data2)));
		SHOW("------ xmldata ------");
		SHOWU(test.get_data());
		SHOW("----- end xmldata -----");
		return 0;
	}
	if(data == "request_users_list")
	{
		xml_implode test(Glib::locale_to_utf8(data));	
		SHOW("Podaj uid:");
		cin >> data;
		assert(test.set_uid_request_users_list(Glib::locale_to_utf8(data)));
		SHOW("------ xmldata ------");
		SHOWU(test.get_data());
		SHOW("----- end xmldata -----");
		return 0;
	}
	if(data == "request_users_states")
	{
		xml_implode test(Glib::locale_to_utf8(data));	
		SHOW("Podaj uid:");
		cin >> data;
		assert(test.set_uid_request_users_states(Glib::locale_to_utf8(data)));
		SHOW("------ xmldata ------");
		SHOWU(test.get_data());
		SHOW("----- end xmldata -----");
		return 0;
	}
	SHOW("typ xml-a nie rozpoznany!");
	return 0;
}
