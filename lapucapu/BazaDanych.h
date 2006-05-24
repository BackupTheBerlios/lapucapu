#ifndef BAZA_H_
#define BAZA_H_

#include <mysql++.h>

class BazaDanych
{
private:
	static char* def_db_name;
	static char* def_host; 
	static char* def_db_user_name;
	static char* def_db_user_pass;
	static char* def_db_mysql_socket;

public:
	BazaDanych(const char* db_user_name, const char* db_user_pass,
		const char* db_name, const char* host);
	BazaDanych();
	~BazaDanych();
	void polacz();
	
	mysqlpp::Connection* con;		//polaczenie z baza
	
	int add_usr();
	int remove_user( int user_id );
	int push_message( int user_id, std::string& message );
	int pop_message( int user_id );	//biebiera najstarsza wiadomosc
	std::vector<int> get_contacts_list(int owner);
};

#endif /*BAZA_H_*/
