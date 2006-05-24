#include "post_office.h"
#include <sys/types.h>
#include <sys/socket.h>

/**
 * Constructors/Destructors
 */
post_office::post_office(std::string sserwer_ip, int sport) {
	
	serwer_ip = sserwer_ip;
	port = sport;
	
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		this->critical_error("Failed to create socket");
	}
	
	/* Construct the server sockaddr_in structure */
    memset(&server, 0, sizeof(server));       /* Clear struct */
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = inet_addr(serwer_ip.c_str());  /* IP address */
    server.sin_port = htons(port);       /* server port */
    
    /* Establish connection */
    connected = true;
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
      connected = false;
      this->set_last_error("Failed to connect with server");
    }
    
    // to trzeba zabezpieczyc ??
    fcntl(sock, O_NONBLOCK);
}

/**
 * 
 */
bool post_office::disconnect() {
	if(connected)
	{
		close(sock);
		connected = false;
		return true;
	}
	this->set_last_error("Not connected");
	return false;
}


/**
 * 
 */
bool post_office::push_data (Glib::ustring data) {
	
	if(connected)
	{
		/* Send the word to the server */
	    int len = strlen(data.c_str());
	    if (send(sock, data.c_str(), len, 0) != len) {
	      this->set_last_error("Mismatch in number of sent bytes");
	      return false;
	    }
		return true;
	}
	this->set_last_error("Not connected");
	return false;
}

/**
 * 
 */
Glib::ustring post_office::pull_data () {

	Glib::ustring data = "";
	
	if(connected)
	{
			
		char buffer[512];
		int bytes = 0;
			
		bytes = recv(sock, buffer, 512, MSG_DONTWAIT);
		if(bytes < 0)
		{
			if( errno != EAGAIN )
			{			
	  			this->set_last_error("Failed to receive bytes from server");
			}
			return "";
		}
		
		data.append(buffer, bytes);	
		return data;
	}
	this->set_last_error("Not connected");
	return "";
}

/**
 * 
 */
std::string post_office::get_serwer_ip ( ) {
	return serwer_ip;
}

/**
 * 
 */
int post_office::get_port() {
	return port;	
}


/**
 * 
 */
bool post_office::is_connected() {
	return connected;	
}
	
	