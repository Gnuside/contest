/*
 ============================================================================
 Name				: server.c
 Author			: avm
 Version		 :
 Copyright	 :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void fin (int i);
void appli(int no_client_socket);
char buffer[512];
int ma_socket;

int main ( void )
{
	int client_socket;
	struct sockaddr_in mon_address, client_address;
	int mon_address_longueur, lg;

	bzero(&mon_address,sizeof(mon_address));
	mon_address.sin_port = htons(30000);
	mon_address.sin_family = AF_INET;
	mon_address.sin_addr.s_addr = htonl(INADDR_ANY);

	/* creation de socket */
	if ((ma_socket = socket(AF_INET,SOCK_STREAM,0))== -1) {
		printf("Can't create the socket.\n");
		exit(2);
	}

	signal(SIGINT,fin);
	/* bind serveur - socket */
	bind(ma_socket,(struct sockaddr *)&mon_address,sizeof(mon_address));

	/* ecoute sur la socket */
	listen(ma_socket,5);

	/* accept la connexion */
	mon_address_longueur = sizeof(client_address);

	while(1) {
		client_socket = accept( ma_socket, (struct sockaddr *)&client_address, &mon_address_longueur);

		if (fork() == 0) {
			close(ma_socket);

			lg = read(client_socket,buffer, 512);
			printf("Data received: %s\n",buffer);
			sprintf(buffer,"%s du serveur",buffer);
			write(client_socket,buffer, 512);
			shutdown(client_socket,2);
			close(client_socket);
			exit(2);
		}
	}

	shutdown(ma_socket,2);
	close(ma_socket);

	exit(0);
}


void appli(int no_client_socket) {
	printf("ici le serveur\n");
}

void fin(int i) {
	shutdown(ma_socket,2);
	close(ma_socket);
}
