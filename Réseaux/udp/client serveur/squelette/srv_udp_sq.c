#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int creerSocket(int, int *, struct sockaddr_in *);
void service(int);

main()
{
	struct sockaddr_in ad;
	int sock, portUDP;
	
	portUDP=2224;

	if((sock = creerSocket(SOCK_DGRAM, &portUDP, &ad)) == -1)
	{
		perror("ERREUR fin de programme !\n");
		exit(1);
	}
	printf("Socket UDP attachée au port : %d\n", ntohs(ad.sin_port));
	service(sock);
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

void service(int sock)
{
	struct sockaddr_in adClientUDP;
	char requete[80], reponse[80];
	int lg, nb;

	lg=sizeof(adClientUDP);
	nb=recvfrom(sock, requete, sizeof(requete), 0, (struct sockaddr*)&adClientUDP, &lg);
	if(nb==-1)
	{
		perror("ERREUR:");
		exit(1);
	}
	requete[nb]='\0';
	printf("message de %d carac recu :%s\n", nb, requete);
	printf("Socket client UDP attachée au port : %d\n", ntohs(adClientUDP.sin_port));

	fgets(reponse, sizeof(reponse), stdin);
	if(sendto(sock, reponse, strlen(reponse), 0, (struct sockaddr *)&adClientUDP, sizeof(struct sockaddr_in))==-1)
	{
		perror("ERREUR sendto:");
		exit(1);
	}
}
