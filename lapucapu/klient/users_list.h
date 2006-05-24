#ifndef USERS_LIST_H_
#define USERS_LIST_H_
#include <map>
#include "user.h"
typedef std::map<Glib::ustring, user> users_list;
typedef std::map<Glib::ustring, user>::iterator users_iter;
  
#endif /*USERS_LIST_H_*/
