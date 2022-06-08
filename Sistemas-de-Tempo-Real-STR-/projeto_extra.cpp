
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{

// declarações de variáveis
  pid_t pid1, pid2, pid3;          // definindo um identificador de processo - inteiro longo
  char *mensagem;     // string que contém a mensagem
 
   
  printf("Começando o programa... \n");
  for(int i = 0; i<5; i++){
    sleep(1);
    printf("\nSou o processo Pai %d, filho de %d\n", getpid(), getppid());
   }
  pid1 = fork();   // dividindo o processo em dois
  switch(pid1)
    {
    case -1:       // erro na abertura do processo filho
      exit(1);  

    case 0:        // Parte a ser executada pelo processo Filho1
      for(int i=1;i<6;i++) {
       sleep(1);
       printf("Sou o processo Filho1 %d, filho de %d\n", getpid(), getppid());
      }

pid3 = fork();
switch(pid3)
        {
        case -1:    // erro
           exit(1);
        case 0:        // Parte a ser executada pelo processo Filho2
         for(int i=1;i<9;i++) {
         sleep(1);
         printf("Sou o processo Filho3 %d, filho de %d\n", getpid(), getppid());
         }
        exit(1);
        default:  // última parte do processo Pai
          for(int i=1;i<7;i++) {
             sleep(1);
             printf("Sou o processo Filho1 %d, filho de %d\n", getpid(), getppid());
          }
	  //kill getpid();
        break;
        }

      break;
    default:       // parte a ser executada pelo processo Pai
      for(int i=1;i<3;i++) {
       sleep(1);
       printf("\nSou o processo Pai %d, filho de %d\n", getpid(), getppid());
      }
      pid2 = fork();
      switch(pid2)
      {
        case -1:    // erro
           exit(1);
        case 0:        // Parte a ser executada pelo processo Filho2
         for(int i=1;i<7;i++) {
         sleep(1);
         printf("Sou o processo Filho2 %d, filho de %d\n", getpid(), getppid());
         }
         //kill getpid();
        exit(1);
        default:  // última parte do processo Pai
          for(int i=1;i<5;i++) {
             sleep(1);
             printf("Sou o processo pai %d, filho de %d\n", getpid(), getppid());
          }
	  //kill getpid();

        break;
      }

      break;
    }
  exit (0);

}

