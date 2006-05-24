#include <string>
#include "config.h"

/**
 * Constructors/Destructors
 */
 
/**
 * ta klasa tylko wybiera dane z pliku konfiguracyjnego
 * otwarcie pliku konfiguracyjnego i wczytanie danych do p?l obiektu
 */
 
config::config(std::string s_file_name)
{
	configured = true;
};

/**
 * destruktor
 */
config::~config()
{
	
}

/**
 * Methods
 */
/**
 * 
 */
int config::get_uid() {
	return 1;
}
/**
 * procedura odczytuj?ca chas?o z pliku konfiguracyjnego
 * narazie jest ustawiona na warto?? domy?ln?
 */
std::string config::get_passwd () {
	return "test";
}
/**
 * 
 */
bool config::is_configured () {
	return configured;
}
/**
 * 
 */
std::string config::get_file_name ( ) {
	return file_name;
}
/**
 * 
 */
void config::set_file_name (std::string s_file_name) {
	this->file_name = s_file_name;
}

