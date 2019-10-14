#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

main(){
  int nb;
  char buff[10000];
  do{
  nb = read(STDIN_FILENO, buff, sizeof(buff));
  buff[nb]='\0';
   }
  while(STDIN_FILENO != '\0');
  write(1, buff, nb);
  write(STDIN_FILENO, buff, nb);
  close(STDIN_FILENO);
}
