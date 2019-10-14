#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int creerSocket(int, int *, struct sockaddr_in *);

void eleminerZombie(int sig){
  printf("\nFin d'un processus de service\n");
  wait(NULL);
}

main(int argc, char *argv[]){
  struct sockaddr_in adresse;
  int sockEcoute, sockService, portTCP, lg;
  
  if(argc != 3){
    fprintf(stderr,"Erreur sur le nombre de parametre! nport service\n");
    exit(2);
  }
  
  if(access(argv[2], X_OK) != 0){
      fprintf(stderr,"Fichier %s non executable\n",argv[2]);
      exit(2);
  }
  
  if(fork() != 0)exit(0);
  setsid();
  printf("Serveur de pid %d lancé \n",getpid());
  
  signal(SIGCHLD, eleminerZombie);
  portTCP=atoi(argv[1]);
  
  lg=sizeof(adresse);
  
  if((sockEcoute=creerSocket(SOCK_STREAM, &portTCP, &adresse)) == -1){
    perror("Erreur fin de programme!\n");
    exit(1);
  }
  
  if(listen(sockEcoute, 5) == -1){
    perror("listen\n");
    exit(2);
  }
  
  while(1){
    sockService=accept(sockEcoute, (struct sockaddr*)&adresse, &lg);
    printf("Connection accepté\n");
    if(fork() == 0){
      close(sockEcoute);
      dup2(sockService, STDIN_FILENO);
      close(sockService),
      execlp(argv[2], argv[2], NULL);
      perror("exec\n");
      exit(2);
    }
    close(sockService);
  }
  
}

int creerSocket(int type, int *portTCP, struct sockaddr_in *ptr_adresse){
  int desc;
  int longueur= sizeof(struct sockaddr_in);
  
  desc=socket(AF_INET, type, 0);
  if(desc == -1){
    perror("ERREUR création de socket impossible !\n");
    return (-1);
  }
  
  ptr_adresse->sin_family=AF_INET;
  ptr_adresse->sin_port=htons(*portTCP);
  ptr_adresse->sin_addr.s_addr=INADDR_ANY;
  
  if((bind(desc, (struct sockaddr*)ptr_adresse, longueur)) == -1){
    perror("ERREUR Attachement de la socket impossible !\n");
    close(desc);
    exit(-1);
  }
  
  if(ptr_adresse != NULL){
    getsockname(desc, (struct sockaddr*)ptr_adresse, &longueur);    
  }
  
  return desc;
}
