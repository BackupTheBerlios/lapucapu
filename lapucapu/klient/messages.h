#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <iostream>
#include <glibmm.h>

void unreconized_xml(std::string reazon);

#define VERBOSE

#ifdef VERBOSE
#define SHOW(co) { std::cout << co << std::endl;}
#define SHOWU(co) { std::cout << Glib::locale_from_utf8(co) << std::endl;}
#endif //VERBOSE

#endif //MESSAGES_H
