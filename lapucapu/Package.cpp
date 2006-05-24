#include "Package.h"

	Package::Package(Glib::ustring nag, Glib::ustring wiad) : naglowek(nag), wiadomosc(wiad)
	{
		std::cout << "Package::Package()" <<std::endl;
	};
	
	Package::~Package() 
	{
		std::cout << "Package::~Package()" <<std::endl;
	};
			
	Glib::ustring Package::header() const { return naglowek; }
	
	void Package::header(Glib::ustring nag) { naglowek = nag; };
	
	Glib::ustring Package::body() const { return wiadomosc; }
	
	void Package::body(Glib::ustring body) { wiadomosc = body; };
	
	void Package::receiver(int user_id) { adresaci.push_back(user_id); };
	
	void Package::receivers(std::vector<int> lista_adresaci) { adresaci = lista_adresaci; };
	
	void Package::reset()
	{
		it = adresaci.begin();
	}
	
	int Package::receiver()
	{
		if( it != adresaci.end() )
		{
			return *it++;
		}
		else 
		{
			it = adresaci.begin();
			return -1;
		}
	};
	
	int Package::show_all()
	{		std::cout << "naglowek: " << naglowek << std::endl;
		
		for(std::vector<int>::iterator it = adresaci.begin(); it != adresaci.end(); it++)
		{
			std::cout << "adresat: " << *it << std::endl;
		}
		
		std::cout << "wiadomowsc: " << wiadomosc << std::endl;
		
		return 0;
	};
	
