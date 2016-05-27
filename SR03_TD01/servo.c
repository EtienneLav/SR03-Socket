#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "defobj.h"

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;


//void client(char ** clientName, int port) {
int main(int argc, char ** argv) {
	int sock;
	int sock_err, status;
	SOCKADDR_IN sin;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    obj object;

	if (sock == -1){
		perror("socket");
		return 0;	
	};

	bzero(&sin, sizeof(sin));


	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(atoi(argv[1]));	
	sin.sin_family = AF_INET;

	sock_err = bind(sock, (SOCKADDR*)&sin, sizeof(sin));
    
	if (sock_err == INVALID_SOCKET) {
		perror("bind");
		return 0;
	}

	sock_err = listen(sock, 5);
	if (sock_err == SOCKET_ERROR) {
		perror("listen");
		return 0;
	}

	SOCKET csock;
	socklen_t crecsize = sizeof(sin);
		
	while (1) {
		csock = accept(sock, (SOCKADDR*)&sin, &crecsize);
	
		int pid1;
		if((pid1 = fork()) < 0) {
			printf("Failed fork");
			return 0;	
		}
	
		if (pid1 == 0) {
		// Processus fils.
			recv(csock, &object, sizeof(object),0);
            
            printf("objet : %s\t%s\t%d\t%d\t%f\n", object.id, object.dsc, object.ii, object.jj, object.dd);
            
		}

		if (pid1 > 1) {
		// Processus père.
		waitpid(pid1,&status, WUNTRACED);
		}
	}
}	

