#include <mysql++.h>
#include "BazaDanych.h"

//defaultowe parametry do polaczenia z baza

char* BazaDanych::def_db_name 		= "komin";
char* BazaDanych::def_host 			= "zyleta";
char* BazaDanych::def_db_user_name 	= "komind";
char* BazaDanych::def_db_user_pass 	= "komind";
char* BazaDanych::def_db_mysql_socket="/home/mysql/mysql.sock";

BazaDanych::BazaDanych(const char* db_user_name,
	const char* db_user_pass,
	const char* db_name,
	const char* host)
{
	//std::cout << "Baza::Baza(parametry)" << std::endl;
	con = new mysqlpp::Connection(mysqlpp::use_exceptions);
	try
	{	
		con->connect(db_name,host,db_user_name,db_user_pass,
		0,0,60,def_db_mysql_socket,0);	
	}
	catch( mysqlpp::BadQuery& er )
	{
		std::cout << "wystapil blad:" << er.what() << std::endl;
	};
};

BazaDanych::BazaDanych()
{
	//std::cout << "Baza::Baza()" << std::endl;
	polacz();
}

BazaDanych::~BazaDanych()
{
	std::cout << "Baza::~Baza()" << std::endl;
};

void
BazaDanych::polacz()
{
	con = new mysqlpp::Connection(mysqlpp::use_exceptions);
	try
	{	
		con->connect(def_db_name, def_host, def_db_user_name, def_db_user_pass,
		0,0,60,def_db_mysql_socket,0);	
	}
	catch( mysqlpp::BadQuery& er )
	{
		std::cout << "wystapil blad:" << er.what() << std::endl;
	};
};
	
	
	int BazaDanych::add_usr()
	{
		return 0;
	};
	int BazaDanych::remove_user( int user_id )
	{
		return 0;
	};
	int BazaDanych::push_message( int user_id, std::string& message )
	{
		return 0;
	};
	int BazaDanych::pop_message( int user_id )
	{
		return 0;
	};	//biebiera najstarsza wiadomosc
	
	/*
	 * Metoda zwraca liste kontaktow z baxy.
	 * Trzeba sie dogadac co do samej bazy.
	 * Ja za niedlugo puszcze jakis pliczek z baza.
	 */
	std::vector<int> BazaDanych::get_contacts_list( int owner )
	{
		std::vector<int> contacts_list;
		mysqlpp::Query query = con->query();		
		query << "SELECT `contact` FROM `contacts` WHERE `user_id`=" << mysqlpp::quote_only << owner;
		std::cout << "Query: " << query.preview() << std::endl;
		
		mysqlpp::Result result;
		result = query.store();
		if( result )
		{
			mysqlpp::Row row;
			mysqlpp::Row::size_type i;
			
			/*
			 * teraz zrobilem tak jak jest w manualu
			 * warunkiem jest jednak wylaczenie wyjatkow
			 * nie wiem czy to dobre posuniecie
			 */
			result.disable_exceptions();
			for (i=0; row=result.at(i); ++i)
			{
				contacts_list.push_back( row.at(0) );
			};	
		}
		else
		{
			/*
			 * dziwne to jest ale nawet jak lapie wyjatki
			 * to one i tak zyja wlasnym zyciem i mi progsa wywali... musze to jeszcze obadadc
			 */
			std::cout << "nie udalo sie wybrac wynikow z bazy: " << query.error() << std::endl;
		};
		
		return contacts_list;	
	};
	