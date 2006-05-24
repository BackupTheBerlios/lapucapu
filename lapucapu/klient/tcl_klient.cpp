#include <fstream>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include <tcl.h>

#ifndef TCL_LOCAL_APPINIT
#define TCL_LOCAL_APPINIT Tcl_AppInit    
#endif


#include <glibmm.h>
#include <string>
#include <assert.h>
#include <glib.h>

#include "xml_explode.h"
#include "xml_implode.h"
#include "users_list.h"
#include "user.h"
#include "post_office.h"
#include "messages.h"

using namespace std;
int *main_argc;
char **main_argv;

users_list moja_lista;
user ja;
bool zalogowano = false;
post_office * poczta = 0;
static GMutex *socket_access = NULL;

GThread* odbior_danych;

void mutex_init() 
{
	g_assert (socket_access == NULL);
	socket_access = g_mutex_new ();
}

// funkcja sprawdzajaca czy jakies dane sa na porcie 

void * fetch_data_from_server(void *)
{
	for(;;)
	{
		g_mutex_lock (socket_access);
		Glib::ustring data = poczta->pull_data();
		g_mutex_unlock (socket_access);
		
		if(poczta->is_err())
		{
			cout << "Watek: fetch_data_from_server zakonczony bo:" << endl;
			cout << poczta->get_last_error() << endl;
			delete poczta;
			poczta = 0;
			g_thread_exit(NULL);
		}
		
		if(data.length())
		{
			SHOW("Przybyly jakies dane...");
			SHOW("---------------------------------------");
			SHOWU(data);
			SHOW("---------------------------------------");
			
			// sprawdzenie zawartosci danych	
		}
		
		sleep(1);
	}	
}

void push_data(Glib::ustring data)
{
		g_mutex_lock (socket_access);
		poczta->push_data(data);
		g_mutex_unlock (socket_access);
}

int hello(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	cout << "nazwa wywolanego polecenia: " << argv[0] << endl;
	cout << "Hello my maser :D " << endl << "ilosc argumentow:" << argc-1 << endl << "Listuje argumenty" << endl;
	
	for(int i = 1; i < argc; i++)
	{
		cout << "argument nr " << i << " :" << argv[i] << endl;
	}
	return 0;
};


int test(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	cout << "Test!!" << endl;
	
	return 0;
};

int help(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	Glib::ustring data;

	ifstream is;
	is.open ("help.txt", ios::binary );

	if(!is)
		cout << "Klopot z otwarciem pliku help.txt" << endl;

	cout << is.rdbuf();
	
	return 0;
};

int connect(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 3)
	{
		cout << "Uzycie: connect <serwer ip> <port num>" << endl;
		return -1;
	}
	if(!poczta)
	{
		string str1;
		str1.append(argv[1]);
		cout << "Serwer ip: " << str1 << endl;
		int port = atoi(argv[2]);
		cout << "Port num: " << port << endl;
		poczta = new post_office(str1, port);
		if(poczta->is_err())
		{
			cout << poczta->get_last_error() << endl;
			return -1;
		}
		
		odbior_danych = g_thread_create(&fetch_data_from_server, NULL, true, NULL);
		return 0;
	}
	else
	{
		cout << "Polaczenie zostalo juz utworzone" << endl;
		return -1;
	}
};

int disconnect(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 1)
	{
		cout << "Uzycie: disconnect" << endl;
		return -1;
	}
	if(!poczta)
	{
		cout << "Nie nawi?zano jeszcze po??czenia" << endl;
		return -1;	
	}
	if(!poczta->disconnect())
	{
		cout << poczta->get_last_error() << endl;
		return -1;	
	}
	
	return 0;
};

int login(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 3)
	{
		cout << "Uzycie: login <uid> <passwd>" << endl;
		return -1;
	}
	else
	{
		// tak jest ?adniej chyba :P
		if(!poczta || !poczta->is_connected())
		{
			cout << "Nie nawiazano jeszcze polaczenia" << endl;
			return -1;
		}
		
		string str1;
		str1.append(argv[1]);
		
		string str2;
		str2.append(argv[2]);
		
		cout << "Proba zalogowania z uid = " << str1 << endl;
		xml_implode skladaj("request_login");
		skladaj.set_login_data(Glib::locale_to_utf8(str1), Glib::locale_to_utf8(str2));
		push_data(skladaj.get_data());
	}
	return 0;
};

int message(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 3)
	{
		cout << "Uzycie: message <receiver uid> <message content>" << endl;
		return -1;
	}
	else
	{
		if(!poczta || !poczta->is_connected())
		{
			cout << "Nie nawi?zano jeszcze po??czenia" << endl;
			return -1;
		}
		string str1;
		str1.append(argv[1]);
		cout << "receiver uid: " << str1 << endl;
		string str2;
		str2.append(argv[2]);
		cout << "message body: " << str2 << endl;
		
		xml_implode skladaj("message");
		
		//trza sprawdzi? czy zalogowany i przekazac uid !!
		//ale musze miec co? co wysle do mnie pliczek xml
		skladaj.set_message_from("00000001");
		
		skladaj.set_message_to(Glib::locale_to_utf8(str1));
		skladaj.set_message_body(Glib::locale_to_utf8(str2));
		push_data(skladaj.get_data());
	}
	return 0;
};

int add_user(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 2)
	{
		cout << "Uzycie: add_user <uid>" << endl;
		return -1;
	}
	else
	{
		if(!poczta || !poczta->is_connected())
		{
			cout << "Nie nawi?zano jeszcze po??czenia" << endl;
			return -1;
		}
		user nuser;
		string str1;
		str1.append(argv[1]);
		cout << str1 << endl;
		nuser.set_uid(Glib::locale_to_utf8(str1));
		moja_lista[nuser.get_uid()] = nuser;
	}
	return 0;
};

int rm_user(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 2)
	{
		cout << "Uzycie: rm_user <uid>" << endl;
		return -1;
	}
	else
	{
		if(!poczta || !poczta->is_connected())
		{
			cout << "Nie nawi?zano jeszcze po??czenia" << endl;
			return -1;
		}
		string str1;
		str1.append(argv[1]);
		cout << "Usuwam: " << str1 << endl;
		moja_lista.erase(Glib::locale_to_utf8(str1));
	}
	return 0;
};

int new_user(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 2)
	{
		cout << "Uzycie: new_user <passwd>" << endl;
		return -1;
	}
	else
	{
		if(!poczta || !poczta->is_connected())
		{
			cout << "Nie nawi?zano jeszcze po??czenia" << endl;
			return -1;
		}
		string str1;
		str1.append(argv[1]);
		xml_implode skladaj("request_add_user");
		skladaj.set_new_user_passwd(Glib::locale_to_utf8(str1));
		push_data(skladaj.get_data());
	}
	return 0;
};

int show_users(ClientData clientData, Tcl_Interp *interp, int argc, CONST char *argv[])
{
	if(argc != 1)
	{
		cout << "Uzycie: show_users" << endl;
		return -1;
	}
	users_iter ui;
	for(ui = moja_lista.begin(); ui != moja_lista.end(); ++ui)
	{
		cout << "**********" << endl;
  		cout << "User: ";
  		SHOWU((*ui).second.get_uid());
  		cout << "State: ";
  		SHOWU((*ui).second.get_state());
  		cout << "Opis: ";
  		SHOWU((*ui).second.get_opis());	
  		cout << "**********" << endl;	
	}
	return 0;
};

int Tcl_AppInit(Tcl_Interp *interp)/* Interpreter for application. */
{
    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

    /*
     * Call the init procedures for included packages.  Each call should
     * look like this:
     *
     * if (Mod_Init(interp) == TCL_ERROR) {
     *     return TCL_ERROR;
     * }
     *
     * where "Mod" is the name of the module.
     */

    /*
     * Call Tcl_CreateCommand for application-specific commands, if
     * they weren't already created by the init procedures called above.
     */
     
	Tcl_CreateCommand(interp, "hello", hello, NULL, NULL);
	Tcl_CreateCommand(interp, "test", test, NULL, NULL); 
	Tcl_CreateCommand(interp, "connect", connect, NULL, NULL);
	Tcl_CreateCommand(interp, "disconnect", disconnect, NULL, NULL);
	Tcl_CreateCommand(interp, "login", login, NULL, NULL);
	
	//fuck to pisa?em zaspany chyba :P
	Tcl_CreateCommand(interp, "add_user", add_user, NULL, NULL);
	
	Tcl_CreateCommand(interp, "show_users", show_users, NULL, NULL);
	Tcl_CreateCommand(interp, "new_user", new_user, NULL, NULL);
	
	Tcl_CreateCommand(interp, "message", message, NULL, NULL);
	Tcl_CreateCommand(interp, "help", help, NULL, NULL);
	
    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */

    return TCL_OK;
};

int hdhdmain(int argc, char* argv[])
{
	cout << "Witaj w tcl_klient, napisz help aby dowiedziec sie wiecej." << endl;
	if (!g_thread_supported ()) g_thread_init (NULL);
	mutex_init();
	Tcl_Main(argc, argv, TCL_LOCAL_APPINIT);
	return 0;
}
