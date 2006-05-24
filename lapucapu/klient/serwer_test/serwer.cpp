#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <unistd.h>
#include <netinet/in.h>

#include <string>
#include <iostream>
#include <glibmm.h>

#define MAXPENDING 5    /* Max connection requests */
#define BUFFSIZE 512

using namespace std;

void Die(char *mess) { perror(mess); exit(1); }

void HandleClient(int sock) {
    char buffer[BUFFSIZE];
    int received = -1;
    /* Receive message */
    for(;;)
    {
//    	cout << "Waiting for data.." << endl;
    	received = recv(sock, buffer, BUFFSIZE, 0);
    	if (received < 0) {
      		Die("Failed to receive initial bytes from client");
    		}
    	else
    	{
    		if(received > 0)
    		{
    			string str;
    			str.append(buffer,received);
    			cout << "---------------------------------------" << endl;
    			cout << Glib::locale_from_utf8(str);
    			cout << "---------------------------------------" << endl;
    		}
    	}
    }
  }
  
int dsfdsmain(int argc, char *argv[]) {
    int serversock, clientsock;
    struct sockaddr_in echoserver, echoclient;

    if (argc != 2) {
      fprintf(stderr, "USAGE: echoserver <port>\n");
      exit(1);
    }
    /* Create the TCP socket */
    if ((serversock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      Die("Failed to create socket");
    }

/* Construct the server sockaddr_in structure */
    memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
    echoserver.sin_family = PF_INET;
    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
    echoserver.sin_port = htons(atoi(argv[1]));       /* server port */
    
/* Bind the server socket */
          if (bind(serversock, (struct sockaddr *) &echoserver,
                                       sizeof(echoserver)) < 0) {
            Die("Failed to bind the server socket");
          }
          /* Listen on the server socket */
          if (listen(serversock, MAXPENDING) < 0) {
            Die("Failed to listen on server socket");
          }
          
/* Run until cancelled */
            while (1) {
              unsigned int clientlen = sizeof(echoclient);
              /* Wait for client connection */
              if ((clientsock =
                   accept(serversock, (struct sockaddr *) &echoclient,
                          &clientlen)) < 0) {
                Die("Failed to accept client connection");
              }
              fprintf(stdout, "Client connected: %s\n",
                              inet_ntoa(echoclient.sin_addr));
              HandleClient(clientsock);
            }
          }


