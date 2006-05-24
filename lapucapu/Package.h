#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <iostream>
#include <glibmm.h>
#include <vector>
#include <iterator>

class Package
{
	Glib::ustring naglowek;
	Glib::ustring wiadomosc;
	std::vector<int> adresaci;
	std::vector<int>::iterator it;
public:
	Package(Glib::ustring nag="", Glib::ustring wiad="");
	~Package();	
	Glib::ustring header() const;
	void header(Glib::ustring nag);
	Glib::ustring body() const;
	void body(Glib::ustring body);
	void receiver(int user_id);
	int receiver();
	void receivers(std::vector<int> lista_adresaci);
	void reset();
	//std::vector<int>::iterator Package::receivers_begin();
	//std::vector<int>::iterator Package::receivers_end();
	int show_all();
};

#endif



