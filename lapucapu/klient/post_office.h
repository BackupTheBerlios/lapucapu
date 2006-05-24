#ifndef POST_OFFICE_H
#define POST_OFFICE_H

#include <fcntl.h>
#include <errno.h>

#include <stdio.h>          /* stderr, stdout */
#include <netdb.h>          /* hostent struct, gethostbyname() */
#include <arpa/inet.h>      /* inet_ntoa() to format IP address */
#include <netinet/in.h>     /* in_addr structure */

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#include <glibmm.h>
#include <string>

#include "myerror.h"
#include "messages.h"

class post_office: public myerror {
/**
 * Public stuff
 */
public:
	/**
	 * Fields
	 */
	
	post_office(std::string sserwer_ip, int sport);
	
	/**
	 * 
	 */
	bool disconnect();
	
	/**
	 * 
	 * @param data 
	 */
	bool push_data (Glib::ustring data);
		
	
	/**
	 * 
	 *
	 */
	Glib::ustring pull_data ();
		
	/**
	 * 
	 */
	std::string get_serwer_ip ( );
		
	
	/**
	 * 
	 */
	int get_port();
	
	/**
	 * 
	 */
	bool is_connected();
	

/**
 * Private stuff
 */
private:
	/**
	 * Fields
	 */
	std::string serwer_ip;
	int port;
	int sock;
	struct sockaddr_in server;
	bool connected;
	/**
	 * 
	 */
	/**
	 * Accessor Methods
	 */
		
	
	/**
	 * Operations
	 */
};
#endif //POST_OFFICE_H

