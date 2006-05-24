#include <glib.h>
#include "ObslugaKlienta.h"

	ObslugaKlienta::ObslugaKlienta(OnlineUsersList& oul, int usr_id, int socket_desc) : user_id(usr_id), users_list(oul), cs(socket_desc)
	{
		std::cout << "ObslugaKlienta::ObslugaKlienta()" << std::endl;
		user = users_list[user_id];
		suid = user_id;
	};
	
	
	ObslugaKlienta::~ObslugaKlienta()
	{
		if( users_list[user_id] != NULL)
		{
			//user jest na liscie, a skoro konczymy to go z listy usuwam
			users_list.remove_user(user_id);
		};
	};
	
	
	/*
	 * Metoda wstepnie analizuje przekazany jej string.
	 * Tworzy paczke, uzupelnia pola i zwraca paczke
	 */
	Package ObslugaKlienta::data_to_pkg(const Glib::ustring& xml_data)
	{
		Package pkg;
		
		//------uzyje klasy od mariusza----
		xml_explode* xml_exp = new xml_explode(xml_data);
		Glib::ustring data_type = xml_exp->check_type();
		
		pkg.header(data_type);
		pkg.body(xml_data);
		
		if( data_type == "message" )
		{
			/*
			 * Skladam zwykla paczke z wiadomoscia
			 * Naglowek, adresaci i body
			 */
			pkg.receivers( xml_exp->get_message_uids_to_int() ); 
		}
		else if( data_type == "change_status" )
		{
			/*
			 * Prozba o zmiane statusu. Musze pobrac
			 * z bazy liste kontaktow i tym samym
			 * usupelnic liste adresatow. Paczka
			 * dalej zachowa sie jak zwykla wiadomosc
			 */
			std::vector<int>contacts_list = get_contacts_list( user_id );
			if( !contacts_list.empty() )
			{
				pkg.receivers( contacts_list );
			}
			else
			{
				/*
				 * Lista kontaktow delikwenta jest pust
				 * a wiec nie ma kogo informowac o 
				 * zmianie statusu.
				 */
				 pkg.receiver(-1);
				 
				 //wprzyszlosci mozna zrobic tak:
				 //pkg.is_empty(true);
			};
	
	
		}
		else if( data_type == "add_user" or data_type == "users_list" or data_type == "user_states" )
		{
			/*
			 * - dodaj usera
			 * - pobierz liste kontaktow
			 * - pobierz statusy ludzi z listy
			 * 
			 * Polaczylem to w jedno, bo z mojego punktu widzenia
			 * na tym etapie robie to samo po prostu, czyli:
			 * Uzupelniam naglowek i body. Adresaci zostaja pusci,
			 * a cala tresc jest analizowana potem dalej.
			 * 
			 * Docelowo mysle ze to po prostu zniknie,
			 * bo nie mam tu nic do roboty na tym etapie.
			 */
			 
		}
		return pkg;
	};
	

	/*
	 * Podobnie jak receive_str metoda stoi na poczatku.
	 * Czyli cos odczytamy z socketa to wkladamy to do niej,
	 * a ona juz robi reszte - czyli analizuje, sklada paczke 
	 * i popycha ja dalej.
	 */
	int ObslugaKlienta::send_str(const Glib::ustring& data)
	{
		Package pkg = data_to_pkg(data);
		Glib::ustring header = pkg.header();
		
		/*
		 * dostalem zwykla wiadomosc
		 * wylylam ja
		 * 
		 * dostalem powiadomienia o zmianie statusu
		 * wysylam jak zwykla wiadomosc
		 * bo paczka ma poskladanych juz adresatow
		 */
		if( header == "message" or header == "change_status" )
		{
			send_pkg(pkg);
		}
		
		/*
		 * mam paczke ktora jest zadaniem 
		 * o liste kontaktow
		 */
		else if( header == "users_list" )
		{
		}
		
		/* mam paczke ktora jest zadaniem
		 * o przeslanie statusow wszystkich userow
		 * z listy delikwenta
		 */
		else if( header == "user_states" )
		{
		}
		
		return 0;
	};
	
	
	/*
	 * Wysylam paczke.
	 * Na podstawie naglowka podejmowane sa odpowiednie 
	 * dzialania
	 */
	int ObslugaKlienta::send_pkg(Package& pkg)
	{
		std::string naglowek = pkg.header();
		//jezeli mam status badz zwykla wiadomosc to traktuje to tak samo - po prostu wysylam
		
		pkg.reset();
		int user_id;
		while( (user_id = pkg.receiver()) != -1 )
		{
			if(users_list[user_id] != 0)
			{
				users_list[user_id]->send_data(pkg);
				return 0;
			}
			else
			{
				std::cout << "sory, nie znalalzme usera o id = " << user_id << std::endl;
				return -2;
			};
		};
		return -1;
	};
	
	
	/*
	 * Zadaniem metody jest sygnalizacja ze konczymy.
	 * Metoda wysyla spreparowana paczke do usera zeby
	 * ten wiedzial ze czas konczyc.
	 */
	void ObslugaKlienta::terminate()
	{
		std::cout << "ObslugaKlienta::terminate()" << std::endl;
		close(cs);
		Package* pkg_terminate = new Package("terminate", "");
		user->terminate(*pkg_terminate);
		sleep(1);
	};


	/*
	 * Metoda wyciaga z paczki arkusz xml i zwraca.
	 * Metoda stoi na koncu i ma pracowac juz z gniazdami
	 * - czyli dostajesz string i zapisujesz go do socketa.
	 */
	Glib::ustring ObslugaKlienta::receive_str()
	{
		std::cout << "ObslugaKlienta::receive - odbieram z bufora" << std::endl;
	
	 		Package pkg = user->receive_data();
	 		if( pkg.header() != "terminate" )
	 		{
	 				//znaczy ze nie jest to paczka rozlaczeniowa
	 			return pkg.body();
	 		}
	 		else
	 		{
				//konczymy zabawe
	 			//users_list->logout(user_id);
				return "";
	 		};
	};


/*
 * Jako ze operujemy na jednym sockecie to trzeba
 * to jakos synchronizowac.
 * Pewien nie jestem ale mysle ze jeden mutex wystarczy
 */	
int ObslugaKlienta::my_send(gpointer dane, int dlugosc_danych)
{
	/*
		W tym miejscu mozan pomyslec nad kompresja i/lub szyfrowaniem.
		Mozna np. zrobic zip'a z haslem lub normalnego zip'a a potem
		przepuscic dane przez aes. Mozliwosci jest wiele, wazne jest
		zeby procesz szyfrowania, kompresji przebiegl szybko.
	*/
	g_mutex_lock(wyslij);
	int result = send(cs, dane, dlugosc_danych,0 ); 
	g_mutex_unlock(wyslij);
	return result;
};

int ObslugaKlienta::my_recv(gpointer dane, int dlugosc_danych)
{
	g_mutex_lock(wyslij);
	if(dane!=NULL && dlugosc_danych>0)
	{
		int result = recv(cs, dane, dlugosc_danych, 0);
		g_mutex_unlock(wyslij);
		return result;
	}
	else
	{
		g_mutex_unlock(wyslij);
		return -1;
	};
};
