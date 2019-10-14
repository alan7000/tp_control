#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int creerSocket(int, int *, struct sockaddr_in *);
void clientService(int, struct sockaddr_in *);

main(int argc, char *argv[])
{
	struct sockaddr_in sockClientUDP, sockServeurUDP;
	int descSockUDP, portUDP=0;
	struct hostent *hp;

	if(argc < 3)
	{
		fprintf(stderr, "Erreur sur le nombre de paramètres !\n");
		exit(2);
	}
	
	if((hp=gethostbyname(argv[1]))==NULL)
	{
		fprintf(stderr, "Erreur sur le nom de la machine serveur !\n");
		exit(2);
	}

	if((descSockUDP=creerSocket(SOCK_DGRAM, &portUDP, &sockClientUDP))==-1)
	{
		fprintf(stderr, "Création socket impossible !\n");
		exit(2);
	}

	printf("Client sur le port : %d\n", ntohs(sockClientUDP.sin_port));

	sockServeurUDP.sin_family=AF_INET;
	sockServeurUDP.sin_port=htons(atoi(argv[2]));
	memcpy(&sockServeurUDP.sin_addr.s_addr, hp->h_addr, hp->h_length);

	clientService(descSockUDP, &sockServeurUDP);
}

int creerSocket(int type, int *portUDP, struct sockaddr_in *ptr_adresse)
{
	int desc;
	int longueur=sizeof(struct sockaddr_in);

/* Création de la socket */
	desc = socket(AF_INET, type, 0);
	if(desc == -1)
	{
		perror("ERREUR Création de socket impossible !");
		return(-1);
	}

/* Associe une adresse locale avec un socket (adresse IP <-> n°port) */
	ptr_adresse->sin_family = AF_INET;
	ptr_adresse->sin_port = htons(*portUDP);
	ptr_adresse->sin_addr.s_addr = INADDR_ANY;
	if((bind(desc, (struct sockaddr *)ptr_adresse, longueur))==-1)
	{
		perror("ERREUR Attachement de la socket impossible !");
		close(desc);
		exit(-1);
	}

/* Récuperation de l'adresse effective d'attachement */
	if(ptr_adresse != NULL){
		getsockname(desc, (struct sockaddr *)ptr_adresse, &longueur);
	}
	return desc;
}

void clientService(int sock, struct sockaddr_in *ptr_adresse)
{
	char buffer[80], *ptr;
	int lg, nb;

	printf("Numéro du descripteur socket = %d\n", sock);

	printf("Entrer une chaine :\n");
	fgets(buffer, sizeof(buffer), stdin);
	if(sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)ptr_adresse, sizeof(struct sockaddr_in))==-1)
	{
		perror("ERREUR sendto:");
		exit(1);
	}
	nb=recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)ptr_adresse, &lg);
	buffer[nb]='\0';
	printf("message de %d carac recu : %s\nFIN\n", nb, buffer);
}
