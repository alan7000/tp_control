#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int creerSocket(int, int *, struct sockaddr_in *);

void eliminerZombie(int sig)
{
	printf("Fin d'un processus de service\n");
	wait(NULL);
}

main(int argc, char *argv[])
{
	struct sockaddr_in adresse;
	int SockEcoute, SockService, portTCP, lg;
	
	if(argc != 3)
	{
		fprintf(stderr, "Erreur sur le nombre de paramètres !\n");
		exit(2);
	}
	
	if(access(argv[2], X_OK) != 0)
	{
		fprintf(stderr, "Fichier %s non executable!\n", argv[2]);
		exit(2);
	}

	if(fork()!=0)exit(0);
	setsid();
	printf("Serveur de pid %d lancé\n", getpid());

	signal(SIGCHLD, eliminerZombie);
	portTCP=atoi(argv[1]);

	lg=sizeof(adresse);

	if((SockEcoute=creerSocket(SOCK_STREAM, &portTCP, &adresse)) == -1)
	{
		perror("ERREUR fin de programme !\n");
		exit(1);
	}

	if(listen(SockEcoute, 5) == -1)
	{
		perror("listen");
		exit(2);
	}

	while(1)
	{
		SockService=accept(SockEcoute, (struct sockaddr *)&adresse, &lg);
		printf("Connection acceptée\n");
		if(fork()==0)
		{
			close(SockEcoute);
			dup2(SockService, STDIN_FILENO);
			close(SockService);
			execlp(argv[2], argv[2], NULL);
			perror("exec");
			exit(2);
		}
		close(SockService);
	}
}

int creerSocket(int type, int *portTCP, struct sockaddr_in *ptr_adresse)
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

	/* Associe une adresse locale avec une socket (adresse IP <-> n°port) */
	ptr_adresse->sin_family = AF_INET;
	ptr_adresse->sin_port = htons(*portTCP);
	ptr_adresse->sin_addr.s_addr = INADDR_ANY;
	if((bind(desc, (struct sockaddr *)ptr_adresse, longueur)) == -1)
	{
		perror("ERREUR Attachement de la socket impossible !");
		close(desc);
		exit(-1);
	}

	/* Récuperation de l'adresse effective d'attachement */
	if(ptr_adresse != NULL)
		getsockname(desc, (struct sockaddr *)ptr_adresse, &longueur);
	return desc;
}
