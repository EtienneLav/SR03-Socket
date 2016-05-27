#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include "defobj.h"

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

//void client(char ** clientName, int port) {
int main(int argc, char ** argv) {
    
	SOCKET sock;
	SOCKADDR_IN sin, saddrsev;
	struct hostent *hid;

    //Définit le socket comme passant par internet, en TCP, avec le potocole par défaut
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if ( sock == -1){
		perror("socket");
		return 0;	
	};

    
	bzero(&sin, sizeof(sin));


    //On remplit la structure sin de type sockadd_in avec les infos nécessaire
    //Non nécessaire car ce sera fait automatiquement ... Mais c'est une bonne pratique
    // la première adresse associé au pc qui est libre
    // le premier port libre pour le client
    // passant par internet
    
	sin.sin_addr.s_addr = htons(INADDR_ANY);
	sin.sin_port = 0; // Premier port libre pour le client.
	sin.sin_family = AF_INET;
		
    //bind le socket
	int sock_err;
	sock_err = bind(sock, (SOCKADDR*)&sin, sizeof(sin));
	if (sock_err == INVALID_SOCKET) {
		perror("bind");
		return 0;
	}

	hid = gethostbyname(argv[1]);
	bzero(&saddrsev, sizeof(saddrsev));
	saddrsev.sin_port = htons(atoi(argv[2])); // Premier port libre pour le client.
	saddrsev.sin_family = AF_INET;
	bcopy(hid->h_addr, &(saddrsev.sin_addr.s_addr), hid->h_length);
	
	if(connect(sock, (SOCKADDR*)&saddrsev, sizeof(saddrsev)) == SOCKET_ERROR) {
		perror("connect");
		return 0;
	}

    obj persTab[3] = {{"ident_o1", "description_o1", 11, 12, 10.2345},{"ident_o2", "description_o2", 21, 22, 20.2345},{"ident_o3", "description_o3", 31, 32, 30.2345}};
    
    printf("objet : %s\t%s\t%d\t%d\t%f\n", persTab[1].id, persTab[1].dsc, persTab[1].ii, persTab[1].jj, persTab[1].dd);

	send(sock,&persTab[1],sizeof(persTab[1]), 0);
	close(sock);

}	

