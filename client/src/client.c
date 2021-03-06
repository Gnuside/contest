/*
 ============================================================================
 Name : client.c
 Author : avm
 Version :
 Copyright :
 Description : Simple Client C, Ansi-style
 ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define SERVERNAME "127.0.0.1"

int to_server_socket = -1;

int main ( void ) {
	//char *server_name = SERVERNAME;
	struct sockaddr_in serverSockAddr;
	struct hostent *serverHostEnt;
	long hostAddr;
	//long status;
	char buffer[512];
	long bytes_received;

	bzero(&serverSockAddr,sizeof(serverSockAddr));
	hostAddr = inet_addr(SERVERNAME);

	if ( (long)hostAddr != (long)-1) {
		bcopy(&hostAddr,&serverSockAddr.sin_addr,sizeof(hostAddr));
	} else {
		serverHostEnt = gethostbyname(SERVERNAME);
		if (serverHostEnt == NULL){
			printf("Can't resolve the host.\n");
			exit(2);
		}
		bcopy(serverHostEnt->h_addr,&serverSockAddr.sin_addr,serverHostEnt->h_length);
	}

	serverSockAddr.sin_port = htons(30000);
	serverSockAddr.sin_family = AF_INET;

	/* creation de la socket */
	if ( (to_server_socket = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		printf("Can't create the socket.\n");
		exit(2);
	}

	/* requete de connexion */
	if(connect( to_server_socket,
				(struct sockaddr *)&serverSockAddr,
				sizeof(serverSockAddr)) < 0 ) {
		printf("Can't connect to the server.\n");
		exit(2);
	}

	/* envoie de donne et reception */
	write(to_server_socket,"Toto",4);
	bytes_received = read(to_server_socket,buffer,512);
	printf("Data received : %ld Bytes\n", bytes_received);

	/* fermeture de la connection */
	shutdown(to_server_socket,2);
	close(to_server_socket);

	exit(0);
}

