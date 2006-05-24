#include "User.h"
/*
	Klasa User zawiera w sobie bufor wiadomosci.. jest to nic innego jak interfejs do obslugi bufora - tyle, niewiele wiecej sie tu chyba znajdzie.
*/
	
	User::User(int usr_id, int stat_id ) : user_id(usr_id), status_id(stat_id)
	{
		std::cout << "User::User()" <<std::endl;
		status = "";
		ilosc_klientow++;
		
		//mutexy
		//http://developer.gnome.org/doc/API/glib/glib-threads.html
		if (!g_thread_supported()) g_thread_init (NULL);
		data_cond = g_cond_new();
		data_mutex = g_mutex_new();
		std::cout << "User::User():koniec" <<std::endl;
	};
	
	/*
		Tutaj bede sie musial dobrze zastanowic jak to posprzatac poniewaz: moja wiedza nie jest za duza w tej dziedzinie, absolutnie nie mozna doprowadzicdo wyciekow pamieci !!!
	*/
	User::~User() 
	{
		std::cout << "User::~User()" <<std::endl;
		bufor.clear();
	};

	/*
	void User::set_socket(int sock_desc)
	{
		socket_desc = sock_desc;
	};
	*/
	int User::send_data(Package data)
	{
		/*
		std::cout << "User::send_data()" <<std::endl;
		std::cout << "data header: " << data.header() << std::endl;
		std::cout << "data body: " << data.body() << std::endl;
		*/
		
		g_mutex_lock(data_mutex);
		bufor.push_back(data);
		g_cond_signal(data_cond);
		g_mutex_unlock(data_mutex);
		return 0;
	};
	
	void User::terminate(Package pkg_terminate)
	{
		std::cout << "User::terminate()" << std::endl;
		g_mutex_lock(data_mutex);
		//bufor.pop_front();
		bufor.push_front(pkg_terminate);
		g_cond_signal(data_cond);
		g_mutex_unlock(data_mutex);
	};
	
	Package User::receive_data()
	{
		std::cout << "User::receive_data()" << std::endl;
		
		g_mutex_lock(data_mutex);

		while( bufor.begin() == bufor.end() )
		{
			g_cond_wait(data_cond, data_mutex);
		};
		it = bufor.begin();
		Package pkg = *it;
		bufor.pop_front();
		g_mutex_unlock(data_mutex);
			
		return pkg;
	};
	
	bool User::empty_bufor()
	{
		std::cout << "User::empty_bufor()" << std::endl;
		if( bufor.begin() == bufor.end() )
		{
			//std::cout << "nie mam nic do wyslania :(" << std::endl;
			return true;
		}
		else 
		{
			//std::cout << "mam cos do wyslania :D" << std::endl;
			return false;
		}
	};
	
	
	int User::get_user_id() const
	{
		std::cout << "User::get_user_id()" <<std::endl;
		return user_id;
	};
	
	//tylko do testow.. ;d
	int User::set_status(std::string status)
	{
		this->status = status;
		return 0;
	};
	
	std::string User::get_status()
	{
		return status;
	};
	
	int User::ilosc_klientow = 0;
