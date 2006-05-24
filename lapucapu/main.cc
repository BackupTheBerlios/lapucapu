#include <iostream>
#include <string>
#include <fstream>
#include <libglademm.h>
#include <glib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <mysql++.h>

#include "Package.h"
#include "User.h"
#include "ObslugaKlienta.h"
#include "OnlineUsersList.h"
#include "BazaDanych.h"
#include "ThreadPool.h"

#define SERVER_CODE 1 //na potrzeby chwili

using namespace std;

OnlineUsersList oul;

const int ServicePort=4096;
const int MaxClients=100;

void sender(gpointer data)
{
	cout << "sender" << endl;
	/*
	 * Sender wysyla odpowiednie dane do klienta, zapisuje do jego socketa
	 * czyli odczytuje z bufora od usera
	 * 
	 * Speca przenioslem do ok.
	 * 
	 */
	 
	 /*
	  * W petli odczytujesz ok->receive_str() i zapisujesz do socketa
	  * Jeseli dostaniesz pusty string to znaczy ze konczymy
	  *  - wystarczy wyjsc z funkcji i tyle
	  */
	
	ObslugaKlienta* ok = (ObslugaKlienta*)data;	
	Glib::ustring odebrane;
	while( (odebrane = ok->receive_str()) != "" )
	{
		cout << "receive: " << odebrane << endl;
		if( ok->my_send((gpointer)odebrane.c_str(), odebrane.length()) < 0 )
		{
			perror("SENDER ");
			break ;
		};
		odebrane.empty();
	};
	ok = NULL;
};

void *receiver(gpointer data)
{
	std::cerr << "main: receiver: start\n";
	ObslugaKlienta* ok = (ObslugaKlienta*)data;
	char bufor[512];
	/*
	 * Receiver odczytuje dane z socketa
	 * i wysyla do usera do bufora
	 *
	 * Tego speca rozwiazalem tak ze dalem te pola do ok.
	 * W userze mi to serio nie pasowalo.
	 */
	 
	 /*
	  * W przyszlosci moznapokombinowac zeby to jakos sprzezyc z ok->send_str()
	  * 
	  * Odbierasz, wysylasz. Jezeli przyjdza prozba o rozlaczenie to ja ci o tym powiadomie.
	  * Jezeli stwierdzisz ze cos padlo i trzeba konczyc to ty mnie o tym powiadom
	  * wywolujac ok->terminate();
	  */
	while(true)
	{
		for(int i=0; i<512; i++) 
			bufor[i]='\0';
		if( ok->my_recv(bufor, 512)  )
		{
			perror("receiver: recv ");
			ok->terminate();
			g_thread_exit(NULL);
		}
		ok->send_str(bufor);
	};
	g_thread_exit(NULL);
};



void func(gpointer data, gpointer user_data)
{
	int socket_desc = *(int*)data;
	/*
	 * Odczytuje arkusz xml
	 * mysle ze nie musze tego puscic w petli
	 * bo prozba o logowanie bedzie krotka 
	 */
	char bufor[512];
	if( recv(socket_desc, bufor, 512, 0) <= 0 )
	{
		perror("receiver: recv");
		close(socket_desc);
	} 
	else
	{ 
		//przeprowadzam logowanie
		int user_id, result;
		BazaDanych* db = new BazaDanych();
		try
		{
			result = oul.login(bufor, *(db->con));
		}
		catch(LapucapuException& blad_logowania)
		{
		   /*
			* jezeli tu jestesmy to znaczy ze cos poszlo nie tak
			* nalezy taka informacje odeslac userowi w postaci xml
			* Wiem ze mariusz juz prawie ma skladanie arkuszy wiec
			* uzyje sie jego klas. Poki co mamy tylko informacje o tym
			* co poszlo nie tak
			*/
			blad_logowania.what();
			
			std::cerr << "main.cc:func():bledne dane\n";
			delete db;
			close(socket_desc);
			g_thread_exit(NULL);
		}
		if( result > 0 )
		{
			user_id = result;
			ObslugaKlienta* ok = new ObslugaKlienta(oul, user_id, socket_desc);
			GThread * recv_th = g_thread_create(&receiver, ok, true, NULL);
			sender(ok);
			g_thread_join(recv_th);
		
		/*
		 * Tutaj wywoluje delete, w destruktorze da sie zamykanie gniazda i psuje
		 * Sender w tym miejscu juz wie ze jest po zabawie bo dostal posta paczke
		 *[to siejeszcze zmieni], tym samym tego watku juz niema.
		 *	To mam nadzieje rozwiazuje problem sprzatania.
		 */
		
			delete ok;
			//delete db; // przeniesione nizej, poza blok kontrolowany przez if
		}
		else
		{
			/*
			 * Jezeli cos poszlo nie tak
			 * to musze poskladac odpowiedni xml i odeslac
			 * do delikwenta ktory probowal sie logowac
			 * Prawdopodobnie uzyje tutaj jakies klasy 
			 * do skladanie xml.
			 * Pewnie dostane po glowie od Andrzeja ze xml: za duzo roboty z tym jest :D
			 * a potem od Mariusza ze nie tak tego mam uzyc i zrobic to gdzie indziej :D
			 * Jakby byly jakies propozycje to ja jestem otwarty.
			 */
			switch( result )
			{
				case 0:
					break ;
				case -1:
						break;
				case -2:
						break;
				case -3:
						break;
				case -4:
						break;
			}//switch;
		}//else;
		delete db;
	}//if( recv...;
	g_thread_exit(NULL);
};

int main()
{
	ThreadPool tp(&func);
	struct sockaddr_in ServerSockAddr, ClientSockAddr;
	int ServerSocket, ClientSocket;
	socklen_t alen;
	if( (ServerSocket = socket(PF_INET,SOCK_STREAM,0)) < 0 )
	{
		perror("socket()");
		exit(1);
	}
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_port = htons(ServicePort); //const int ServicePort=...
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY;
	if( bind(ServerSocket,(struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr)) )
	{
		perror("bind()");
		exit(1);
	}
	if( listen(ServerSocket, MaxClients) ) // const int MaxClients=...
	{
		perror("listen()");
		exit(1);
	}
	while(1)
	{
		alen = sizeof(ServerSockAddr);
		ClientSocket = accept(ServerSocket,(struct sockaddr*)&ClientSockAddr, &alen);
		if( ClientSocket < 0 )
		{
			perror("In main while(1) accept");
			close(ClientSocket);
			shutdown(ServerSocket, 3);
			sleep(10);
		}
		else
		{
			//int suid = oul.get_user_id();
			if( !oul.is_aviable() )
			{
				char buf[100];
				sprintf(buf,"Za duzo klientow. wait and try again\n");
				send(ClientSocket,buf, sizeof(buf), MSG_NOSIGNAL);
				close(ClientSocket);
			}
			else
			{
				tp.push((gpointer)&ClientSocket);
				//std::cout << "Nowy user: cs=" << ClientSocket
					//<< " suid=" << suid << std::endl;
			}
		}
	}
	close(ServerSocket);
	return 0;
};
