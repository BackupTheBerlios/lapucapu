#ifndef USER_H_
#define USER_H_

#include <iostream>
#include <deque>
#include <glib.h>
#include "Package.h"

class User
{
	int user_id;				//id obiektu, trzebabedzie podawac z zewnatrz
	int status_id;				//id statusu
	//int socket_desc;
	std::string status;  
	static int ilosc_klientow;
	std::deque<Package> bufor;			//bufor z danymi
	std::deque<Package>::iterator it;		//spacjalizuje iterator kolejki
	
	
	GCond* data_cond;
	GMutex* data_mutex;
	
public:
	User(int usr_id, int stat_id );
	~User();
	int send_data(Package data);
	Package receive_data();
	bool empty_bufor();
	int get_user_id() const;
	int set_status(std::string status);
	std::string get_status();
	//void set_socket(int sock_desc);
	void terminate(Package pkg_terminate);
};

#endif
