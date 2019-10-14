#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int creerSocket(int, int*, struct sockaddr_in*);
void clientService(int, struct sockaddr_in*);
struct sockaddr_in adClientTCP, adServeurTCP;

main(int argc, char *argv[]){
	int SockClient, portTCP=0;
	struct hostent *hp;
	
	if(argc<3)
	{
		fprintf(stderr,"Usage: nom_machine n°port fichier\n");
		exit(2);
	}
	
	if((hp=gethostbyname(argv[1]))==NULL)
	{
		fprintf(stderr,"Erreur sur le nom de la machine serveur !\n");
		exit(2);
		
	}
	
	
	if((SockClient=creerSocket(SOCK_STREAM, &portTCP, &adClientTCP))==-1) 
	{
		fprintf(stderr, "Creation socket impossible !\n");
		exit(2);
	}
	printf("Client sur le port : %d\n",ntohs(adClientTCP.sin_port));
	adServeurTCP.sin_family=AF_INET;
	adServeurTCP.sin_port=htons(atoi(argv[2]));
	memcpy((struct sockaddr *)&adServeurTCP.sin_addr.s_addr, hp->h_addr, hp->h_length);
	
	/* demande de connexion */
	
	if(connect(SockClient,(struct sockaddr *)&adServeurTCP,sizeof(adServeurTCP))==-1)
	{
		fprintf(stderr,"connexion impossible§\n");
		exit(2);
	}
	clientService(SockClient,&adServeurTCP);
}



int creerSocket(int type, int *portTCP, struct sockaddr_in *ptr_adresse) {
	int desc;
	int longueur=sizeof(struct sockaddr_in);

	/*Création de la socket*/
	desc = socket(AF_INET, type, 0);
	if(desc == -1) 
	{
		perror("ERREUR Creation de socket impossible !");
		return(-1);
	}
	
	 /* Associe une adresse locale avec une socket (adresse IP <-> n°port) */

	ptr_adresse->sin_family = AF_INET;
	ptr_adresse->sin_port = htons(*portTCP);
	ptr_adresse->sin_addr.s_addr = INADDR_ANY;
	if((bind(desc, (struct sockaddr *)ptr_adresse, longueur))==-1) {
		perror("ERREUR Attachement de la socket impossible!");
		close(desc);
		exit(-1);
	}
	/* Recuperation de l'adresse effective d'attachement */

	if(ptr_adresse != NULL)
		getsockname(desc, (struct sockaddr *)ptr_adresse, &longueur);

		return desc;
}
void clientService(int sock, struct sockaddr_in *ptr_adresse)
{
	//int ;
	/*FILE *f;
	char buffer[1024];
	
	printf("argv[0]=%s\n",argv[0]);
	if((f=fopen(argv[0],"r"))==NULL)
	{
		perror("ERREUR:"); 
	}*/
	
	char buffer[80], *ptr,car , *zone;
	int nb , size , lg , i , j;

	printf("Numero du descripteur socket = %d\n",sock);

	printf("Entrer une chaine :\n");
	fgets(buffer,sizeof(buffer),stdin);
	if(sendto(sock, zone, strlen(zone), 0, (struct sockaddr *)ptr_adresse, sizeof(struct sockaddr_in))==-1) {
		perror("Erreur sendto:");
		exit(1);

	}if(car=='9'){
		car='0';
	}
	nb=recvfrom(sock, zone, sizeof(zone), 0, (struct sockaddr *) ptr_adresse, &lg);
	buffer[nb]='\0';
	printf("message de %d carac recu : %s\nFIN\n", nb, buffer);
}


	