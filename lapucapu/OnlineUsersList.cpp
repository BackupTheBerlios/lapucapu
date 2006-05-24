#include <glib.h>
#include "OnlineUsersList.h"
#include "klient/xml_explode.h"

	OnlineUsersList::OnlineUsersList()
	{
		std::cout << "OlineUserList::OnlineUsersList()" << std::endl;
		users_count = MaxClients;
		if (!g_thread_supported()) g_thread_init (NULL);
		mutex = g_mutex_new();	
		mutex_login = g_mutex_new();
		std::cout << "OlineUserList::OnlineUsersList():koniec" << std::endl;
	};
	
	OnlineUsersList::~OnlineUsersList()
	{
		std::cout << "OnlineUsersList::~OnlineUsersList()" << std::endl;
	};
	
	/*
	 * Dodaje usera do listy. Tworze pare z it i referencji do obiektu
	 * klasy User. W przyszlosci moze zmieni sie int na string.
	 */
	void OnlineUsersList::add_user(User& user)
	{
		std::cout << "OnlineUsersList::add_user()" <<std::endl;
		g_mutex_lock(mutex);
		
		int id = user.get_user_id();

		it = users_list.find(id);
		if(it == users_list.end()) 
		{
			users_list.insert(std::make_pair(id, user));
			users_count--;
		}
		else std::cout << "nie dodalem usera o id: " << id << std::endl;
		//else throw LapucapuException("wystapil blad podczas dodawania uzytkownika do OUL2");
		g_mutex_unlock(mutex);
	
	};
	
	/*
	 * Usuwam usera z listy.
	 */
	void OnlineUsersList::remove_user(int user_id)
	{
		
		g_mutex_lock(mutex);
		
		std::cout << "OlineUserList::remove_user()" << std::endl;
		it = users_list.find(user_id);
		if( it != users_list.end() )
		{
			users_list.erase(it);
			users_count--;
		}
		else std::cout << "nie znalazlem usera o id: " << user_id << std::endl;
	
		g_mutex_unlock(mutex);
	
	};
	
	/*
	 * Taki maly bajer. Operator zwraca wskaznik
	 * do obiektu klasy User. Jezeli usera nie ma
	 * to zwraam null'a.
	 */
	User* OnlineUsersList::operator[](int user_id)
	{
		
		g_mutex_lock(mutex);
		
		std::cout << "OnlineUserList::operator[]()" << std::endl;
		it = users_list.find(user_id);
		if( it != users_list.end() )
		{	
			g_mutex_unlock(mutex);	
			return &(*it).second;
		}
		else
		{
			
			g_mutex_unlock(mutex);
			std::cout << "shit, nie mam usera id = " << user_id << std::endl; 
			return NULL;
		}
		return 0;
	};
	
	
	/*
	 * Poki co to login dziala ak ze otrzymuje:
	 * - xml_data - czyli to co odczytamy z socketa
	 * - polaczenie do bazy, jednak polaczenie do bazy musi byc nawiazywane kazdorazowo 
	 * dla kazdego watka. Pomylilem sie siarczyscie twierdzac ze jest inaczej.
	 * 
	 * Metoda zwraca:
	 * -1 haslo jest bledne
	 * -2 nie ma w ogole takiego usera w bazie
	 * -3 wystapil jakis inny blad z baza
	 * -4 - jezeli dane to nie prozba o logowanie
	 * login - jezeli wszystko poszlo dobrze to zwracam login zalogowanego wlasnie usera
	 */
	int OnlineUsersList::login(const Glib::ustring& xml_data, mysqlpp::Connection& polaczenie)
	{
		std::cout << "OnlineUsersList::login()" << std::endl;
		
		/*
		 * Wyciagam login i haslo z paczki
		 */
		//----------
		int login;
		Glib::ustring password;
		//-----------
		
		mysqlpp::Query query = polaczenie.query();
		mysqlpp::Result result;
		mysqlpp::Row row;
		try
		{
			xml_explode* xml_exp = new xml_explode(xml_data);
		
		
			
			if( xml_exp->check_type() == "login" )
			{
				login = atoi((xml_exp->get_login_uid()).c_str());
				password = xml_exp->get_login_passwd();
			}
			else
			{
				throw LapucapuException("Blad logowania: to nie jest prozba o logowanie.");
				/*
					std::cout << "to nie jest prozba o logowanie" << std::endl;
				return -4;
				*/
			};
			
			query << "SELECT `password` FROM `users` WHERE `user_id`=" << login << mysqlpp::quote_only;
			result = query.store();
			if( result )
			{
				row = result.at(0);
				if( password == (std::string)row["password"] )
				{
					std::cout << "oka, zalogowales sie !" << std::endl;
					User* usr = new User(login, 0);
					add_user(*usr);
					return login;
				}
				else 
				{
					throw LapucapuException("Blad logowania: bledne haslo");
					/*
					std::cout << "sory, podales bledne haslo !" << std::endl;
					return -1;
					*/
				}
			}
			else 
			{
				throw LapucapuException("Blad logowania: nie jestes zarejestrowanym uzytkownikiem.");
				/*
				std::cout << "sory, nie ma cie w bazie !" << std::endl;
				return -2;
				*/
			}
		}
		
		/*
		 * Najpierw lapie nasze wyjatki
		 * i po prostu wyrzucam je dalej
		 * bo to nic groznego
		 */
		catch( const LapucapuException& e )
		{
			throw e;
		}
		
		/*
		 * Teraz lapie wyjatki z bazy
		 * jezeli takowe wystapia to znaczy
		 * ze mamy problem po naszej stronie 
		 * tego user widziec nie powinien
		 */
		catch( const mysqlpp::BadQuery& er )
		{
			//informacja dla nas
			std::cout << "wystapil blad: " << er.what() << std::endl;
			throw LapucapuException("Blad logowania: przepraszamy, wystapil blad z baza, sprobuj za chwile.");
			;;throw er;
			//return -3;	
		}
		
		/*
		 * Lapie wyjatki z xml parsera
		 */
		catch(const std::exception& e) //narazie bez pomyslu na wyjatek
		{
			//informacja dla nas
			std::cout << "wystapil blad przy logowani, parser xml mowi: " << e.what() << std::endl;
			Glib::ustring blad = "Blad logowania: wystapil blad z arkuszem xml. Parser: ";
			Glib::ustring blad_parsera = e.what();
			Glib::ustring blad_logowania = blad + blad_parsera;
			throw LapucapuException(blad_logowania);
			//throw e;
		}
		/*
		 * lapie cokolwiek innego - jest bardzo zle jak tutaj dojdziemy!
		 */
		catch(...)
		{
			std::cout << "wystapily inne bledy przy logowaniu - pewnie watki namieszaly" << std::endl;
			/*
			 * tutaj musze jeszcze pomyslec jak sie zabawic z tym bledem
			 */
			 throw LapucapuException("Blad logowanie: serwer chwilowo niedostepny, sprobuj za chwile.");
		};
		
		return 0; // zero nie jest poprawnym loginem, czyli oznacza jakis blad
	};

	/*
	 * Usuwam usera z listy
	 */
	int OnlineUsersList::logout(int user_id)
	{
		it = users_list.find(user_id);
		if( it != users_list.end() )
		{
			remove_user(user_id);
			return 0;
		}
		else return 1;
	};
	
	/*
	 * Metoda powstala poto zeby kontrolowac ilosc
	 * userow. Jezeli nie ma wolnych miejsc to
	 * metoda zwroci false.
	 */
	bool OnlineUsersList::is_aviable()
	{
		if( users_count > 0 )
			return true;
		else 
			return false;
	};
