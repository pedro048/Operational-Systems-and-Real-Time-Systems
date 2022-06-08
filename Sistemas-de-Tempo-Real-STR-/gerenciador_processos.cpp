/*
Alunos: Pedro Victor Andrade Alves
        Francisco Kennedi
Projeto: Gerenciador de processos
*/

//taskset -p 0x01 <pid>

#include <signal.h> // definição dos sinais de interrupções
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>     // getpriority(int which, int who)  setpriority(int which, int who, int prio);
#include <sys/resource.h>
#include <string.h>

int op = 0;
char op2 = 'r';
char nomeP[15]; 
char comando[] = "ps -C ";
char cpu[10];
char comando1[] = "taskset -p ";
char pidP[10];
int PID = 0;
int prio = 0;
bool refresh = true;

//string nome_processos;

int main(){
	
	printf("Inicio do gerenciador de processos\n\n");
	while(1){
		refresh = true;
		while(refresh){
			system("clear");
			//system("ps -a -l\n\n ");
			system("ps -e -o s,pid,uname,cmd,pmem,pcpu --sort=-pmem,-pcpu | head -20");
			printf("\n\n");
			printf("Digite 'm' para ver o menu e 'r' para dar refresh: ");
			printf("\n\n");
			scanf("%c", &op2);
			printf("\n\n");		 	
			if(op2 == 'm'){
				refresh = false;
				op2 = 'r';
			}
			
					
		}
		printf("Digite: 1 - Filtar | 2 - Pausar | 3 - Continuar | 4 - Matar | 5 - Muda prioridade | 6 - Escolher CPU\n");
		printf("\n\n");
		printf("Insira a operação:\n");
		scanf("%i", &op);
		
		printf("\n");
		
		switch(op){
    
	      	    	case 1:
				printf("Digite o nome do processo: \n\n");
				scanf("%s", nomeP);
				strcat(comando, nomeP);
				printf("\n\n");
				system(comando);
				printf("\n\n");
				sleep(10);
				
				break;
		    	case 2:
				printf("Digite o PID do processo: \n\n");
				scanf("%d", &PID);
				kill(PID, SIGSTOP);
				break;
		    	case 3:
				printf("Digite o PID do processo: \n\n");
				scanf("%d", &PID);
				kill(PID, SIGCONT);
				break;
	      	    	case 4:
				printf("Digite o PID do processo: \n\n");
				scanf("%d", &PID);
				kill(PID, SIGKILL);
				break;
		    	case 5:
				printf("Digite o PID do processo: \n\n");
				scanf("%d", &PID);
				printf("\n\n");
				printf("Prioridade do processo: %d", getpriority(PRIO_PROCESS, PID));
				printf("\n\n");
				printf("Digite um novo valor de prioridade: \n\n");	
				scanf("%d", &prio);
				printf("\n\n");
				setpriority(PRIO_PROCESS, PID, prio);
				printf("Nova prioridade do processo: %d", getpriority(PRIO_PROCESS, PID));
				printf("\n\n");
				sleep(5);
				break;
		    	case 6:
				printf("Digite o PID do processo: \n\n");
				scanf("%s", pidP);
				printf("\n\n");
				printf("Digite a nova CPU do processo: \n\n");
				printf("\n\n");

				printf("CPU 1 = 0x01\n");
				printf("CPU 2 = 0x02\n");
				printf("CPU 3 = 0x03\n");
				printf("CPU 4 = 0x04\n");
				printf("CPU 5 = 0x05\n");
				printf("CPU 6 = 0x06\n");
				printf("CPU 7 = 0x07\n");
				printf("CPU 8 = 0x08\n");
				printf("\n\n");

				scanf("%s", cpu);
				printf("\n\n");
				strcat(comando1, cpu);
				strcat(comando1, " ");
				strcat(comando1, pidP);
				system(comando1);
				printf("\n\n");
				sleep(10);
				break;
		}		
	   
		
		system("clear");
			
	}
	exit (0);
}
