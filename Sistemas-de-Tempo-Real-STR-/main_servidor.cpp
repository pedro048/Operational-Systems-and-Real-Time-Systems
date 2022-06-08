
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <string>

#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"

#define MULTICAST_ADDR "225.0.0.37"
#define PORT_SCKT_CLIENT 9734
#define PORT_SCKT_SERVER 9734

#define NUM_TRAINS 4
#define MAX_SLEEP_TIME 4
#define MAX_VALUE 4094

using namespace BlackLib;

pthread_mutex_t M1, M2, M3, M4, M5;

void *setSpeed(void *arg);

void *trem1(void *arg);
void *trem2(void *arg);
void *trem3(void *arg);
void *trem4(void *arg);

float sleep_time[NUM_TRAINS];

int main(int argc, char * argv[]) {

 	 int res;
 	 pthread_t thread1, thread2, thread3, thread4, thread5;

 	 void *thread_result;
  
  	 // ------ criando multex M1 ------
  	 res = pthread_mutex_init(&M1, NULL);
  	 if (res != 0)
  	 {
    	 perror("Iniciação do Mutex M1 falhou");
    	 exit(EXIT_FAILURE);
  	 }
 
	  // ------ criando multex M2 ------
	  res = pthread_mutex_init(&M2, NULL);
	  if (res != 0)
	  {
	    perror("Iniciação do Mutex M2 falhou");
	    exit(EXIT_FAILURE);
	  }
	  
	     // ------ criando multex M3 ------
	  res = pthread_mutex_init(&M3, NULL);
	  if (res != 0)
	  {
	    perror("Iniciação do Mutex M3 falhou");
	    exit(EXIT_FAILURE);
	  }

	  // ------ criando multex M4 ------
	  res = pthread_mutex_init(&M4, NULL);
	  if (res != 0)
	  {
	    perror("Iniciação do Mutex M4 falhou");
	    exit(EXIT_FAILURE);
	  }

	  // ------ criando multex M5 ------
	  res = pthread_mutex_init(&M5, NULL);
	  if (res != 0)
	  {
	    perror("Iniciação do Mutex M5 falhou");
	    exit(EXIT_FAILURE);
	  }

	  //------ Thread 1 (executa a fn: trem1) ------
	  res = pthread_create(&thread1, NULL, trem1, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 1 falhou");
	    exit(EXIT_FAILURE);
	  }

	  //------ Thread 2 (executa a fn: trem2) ------
	  res = pthread_create(&thread2, NULL, trem2, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 2 falhou");
	    exit(EXIT_FAILURE);
	  }

	  //------ Thread 3 (executa a fn: trem3) ------
	  res = pthread_create(&thread3, NULL, trem3, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 3 falhou");
	    exit(EXIT_FAILURE);
	  }

	  //------ Thread 4 (executa a fn: trem4) ------
	  res = pthread_create(&thread4, NULL, trem4, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 4 falhou");
	    exit(EXIT_FAILURE);
	  }

	  //------ Thread 5 (executa a fn: setSpeed) ------
	  res = pthread_create(&thread5, NULL, setSpeed, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 5 falhou");
	    exit(EXIT_FAILURE);
	  }

	  // ----- Espera termino das threads
	  res = pthread_join(thread1, &thread_result);
	  if (res != 0)
	  {
	    perror("Juncao da Thread 1 falhou");
	    exit(EXIT_FAILURE);
	  }
	  res = pthread_join(thread2, &thread_result);
	  if (res != 0)
	  {
	    perror("Juncao da Thread 2 falhou");
	    exit(EXIT_FAILURE);
	  }
	  res = pthread_join(thread3, &thread_result);
	  if (res != 0)
	  {
	    perror("Juncao da Thread 3 falhou");
	    exit(EXIT_FAILURE);
	  }
	  res = pthread_join(thread4, &thread_result);
	  if (res != 0)
	  {
	    perror("Juncao da Thread 4 falhou");
	    exit(EXIT_FAILURE);
	  }
	  res = pthread_join(thread5, &thread_result);
	  if (res != 0)
	  {
	    perror("Juncao da Thread 5 falhou");
	    exit(EXIT_FAILURE);
	  }
	
	  //----- destruíndo mutex
	  pthread_mutex_destroy(&M1);
	  pthread_mutex_destroy(&M2);
	  pthread_mutex_destroy(&M3);
	  pthread_mutex_destroy(&M4);
	  pthread_mutex_destroy(&M5);
	  exit(EXIT_SUCCESS);

	  return 0;
}


void *setSpeed(void *arg) {
	int valueEntries[NUM_TRAINS];

	// SOCKET
	int server_sockfd, client_sockfd;
    	size_t server_len;
    	socklen_t client_len;
    	struct sockaddr_in server_address;
    	struct sockaddr_in client_address;
	
	struct ip_mreq mreq;  // para endere�o multicast

	unsigned short porta = 9734;
    
    	//unlink("server_socket");  // remocao de socket antigo
    	if ( (server_sockfd = socket(AF_INET, SOCK_DGRAM, 0) )<0)// cria um novo socket
    {
        printf(" Houve erro na ebertura do socket ");
        exit(1);
    }
	server_address.sin_family = AF_INET;
    	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    	server_address.sin_port = htons(porta);

	server_len = sizeof(server_address);
    
    if(  bind(server_sockfd, (struct sockaddr *) &server_address, server_len) < 0 )
    {
        perror("Houve error no Bind");
        exit(1);
    }

	  // use setsockopt() para requerer inscri��o num grupo multicast
    mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(server_sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    printf(" IPPROTO_IP = %d\n", IPPROTO_IP);
    printf(" SOL_SOCKET = %d\n", SOL_SOCKET);
    printf(" IP_ADD_MEMBERSHIP = %d \n", IP_ADD_MEMBERSHIP);
	while (true) {
		client_len = sizeof(client_address);

		client_len = sizeof(client_address);
		if ( recvfrom(server_sockfd, &valueEntries, sizeof(valueEntries), 0, (struct sockaddr *) &client_address, &client_len) < 0 ) {
			perror("error on RECVFROM()");
			exit(1);
		}

		printf("Server received: (Train 1, Train 2, Train 3, Train 4) = (%d, %d, %d, %d)\n", valueEntries[0], valueEntries[1], valueEntries[2], valueEntries[3]);

		for (int i = 0; i < NUM_TRAINS; i++) {
			sleep_time[i] = (valueEntries[i]*MAX_SLEEP_TIME)/MAX_VALUE;
		}

		sleep(1);
	}
}

void *trem1(void *arg) {
	int sleepTime = MAX_SLEEP_TIME;
	
	BlackGPIO Trem1_T1(GPIO_66, output);
	BlackGPIO Trem1_T2(GPIO_67, output);
	BlackGPIO Trem1_T3(GPIO_69, output);
	
	while (true) {
		sleepTime = sleep_time[0];

		Trem1_T1.setValue(high);
		sleep(sleepTime);
		pthread_mutex_lock(&M1);
		Trem1_T1.setValue(low);
		Trem1_T2.setValue(high);
		sleep(sleepTime);
		Trem1_T2.setValue(low);		
		pthread_mutex_unlock(&M1);	
		pthread_mutex_lock(&M5);
		Trem1_T3.setValue(high);
		sleep(sleepTime);
		Trem1_T3.setValue(low);
		pthread_mutex_unlock(&M5);
  	}
  	pthread_exit(0);
}

void *trem2(void *arg) {
	int sleepTime = MAX_SLEEP_TIME;
	
	BlackGPIO Trem2_T4(GPIO_68, output);
	BlackGPIO Trem2_T5(GPIO_44, output);
	BlackGPIO Trem2_T6(GPIO_26, output);
	BlackGPIO Trem2_T7(GPIO_45, output);

	while (true) {
		sleepTime = sleep_time[1];

		pthread_mutex_lock(&M1);
		Trem2_T7.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M1);
		Trem2_T7.setValue(low);
		
		Trem2_T4.setValue(high);
		sleep(sleepTime);
		pthread_mutex_lock(&M2);
		Trem2_T4.setValue(low);
		Trem2_T5.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M2);
		Trem2_T5.setValue(low);
		pthread_mutex_lock(&M4);
		Trem2_T6.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M4);	
		Trem2_T6.setValue(low);
		
	}
	pthread_exit(0);
}

void *trem3(void *arg) {
	int sleepTime = MAX_SLEEP_TIME;
	
	BlackGPIO Trem3_T8(GPIO_47, output);
	BlackGPIO Trem3_T9(GPIO_27, output);
	BlackGPIO Trem3_T10(GPIO_20, output);
	
	while (true) {
		sleepTime = sleep_time[2];
	
		pthread_mutex_lock(&M2);
		Trem3_T10.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M2);
		Trem3_T10.setValue(low);
		Trem3_T8.setValue(high);
		sleep(sleepTime);
		pthread_mutex_lock(&M3);
		Trem3_T8.setValue(low);
		Trem3_T9.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M3);
		Trem3_T9.setValue(low);
			
	}
	pthread_exit(0);
}

void *trem4(void *arg) {
	int sleepTime = MAX_SLEEP_TIME;
	
	BlackGPIO Trem4_T11(GPIO_60, output);
	BlackGPIO Trem4_T12(GPIO_46, output);
	BlackGPIO Trem4_T13(GPIO_65, output);
	BlackGPIO Trem4_T14(GPIO_61, output);

	while (true) {
		sleepTime = sleep_time[3];

		Trem4_T11.setValue(high);
		sleep(sleepTime);
		pthread_mutex_lock(&M5);
		Trem4_T11.setValue(low);
		Trem4_T12.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M5);
		Trem4_T12.setValue(low);
		pthread_mutex_lock(&M4);
		Trem4_T13.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M4);
		pthread_mutex_lock(&M3);
		Trem4_T13.setValue(low);
		Trem4_T14.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M3);
		Trem4_T14.setValue(low);
		

	}
	pthread_exit(0);
}

/*
Trem1
66
67
69
Trem2
45
68
44
26
Trem3
20
47
27
Trem4
46
65
61
60
*/




