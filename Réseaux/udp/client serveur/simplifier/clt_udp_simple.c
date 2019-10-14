#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

main(){
	struct sockaddr_in sockClientUDP, sockServeurUDP, *ptr_adresse;
    int descSockUDP, portUDP=0, desc;
    struct hostent *hp;

	sockServeurUDP.sin_family=AF_INET;
    sockServeurUDP.sin_port=htons(2224);
    sockServeurUDP.sin_addr.s_addr = inet-addr(127.0.0.1);
    memcpy(&sockServeurUDP.sin_addr.s_addr, hp->h_addr, hp->h_length);
    
    do{
        printf("Numéro descripteur socket : %d\n", descSockUDP);
        
        printf("Entrez une chaine :\n");
        fgets(buffer, sizeof(buffer), stdin);
        if(sendto(descSockUDP, buffer, strlen(buffer), 0, (struct sockaddr *)&sockServeurUDP, sizeof(struct sockaddr_in))==-1){
            perror("Erreur sendto()");
            exit(1);
        }
        
        nb=recvfrom(descSockUDP, buffer, sizeof(buffer), 0, (struct sockaddr *)&sockServeurUDP, &lg);
        buffer[nb-1]='\0';
        printf("Message de %d caractère reçu : %s\nFIN\n", nb, buffer);
    }
    while(strcmp(buffer, "fin"));
    
}
