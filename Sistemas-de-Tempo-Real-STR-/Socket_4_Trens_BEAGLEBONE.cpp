
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
#define PORT_SCKT_CLIENT 1802
#define PORT_SCKT_SERVER 1802

#define NUM_TRAINS 4
#define MAX_SLEEP_TIME 4
#define MAX_VALUE 4094

using namespace BlackLib;

pthread_mutex_t M1, M2, M3, M4, M5;

void *readEntries(void *arg);
void *setSpeed(void *arg);

void *trem1(void *arg);
void *trem2(void *arg);
void *trem3(void *arg);
void *trem4(void *arg);

float sleep_time[NUM_TRAINS];

int main(int argc, char * argv[]) {

 	 int res;
 	 pthread_t thread1, thread2, thread3, thread4, thread5, thread6;

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

	  //------ Thread 5 (executa a fn: readEntries) ------
	  res = pthread_create(&thread5, NULL, readEntries, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 5 falhou");
	    exit(EXIT_FAILURE);
	  }

	  //------ Thread 6 (executa a fn: setSpeed) ------
	  res = pthread_create(&thread6, NULL, setSpeed, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 6 falhou");
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
	  res = pthread_join(thread6, &thread_result);
	  if (res != 0)
	  {
	    perror("Juncao da Thread 6 falhou");
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

void *readEntries(void *arg) {
	int values[NUM_TRAINS];

	// BEAGLEBONE VARIABLES
	ADC train1(AIN0);
	ADC train2(AIN1);
	ADC train3(AIN2);
	ADC train4(AIN3);

	// SOCKET
	int sockfd;
	int len;
	struct sockaddr_in address;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	address.sin_port = htons(PORT_SCKT_CLIENT);
	len = sizeof(address);

	while (true) {
		values[0] = train1.getIntValue();
		values[1] = train2.getIntValue();
		values[2] = train3.getIntValue();
		values[3] = train4.getIntValue();

		sendto(sockfd, values, sizeof(values), 0, (struct sockaddr *) &address, len);

		printf("Client sent: (Train 1, Train 2, Train 3, Train 4) = (%d, %d, %d, %d)\n", values[0], values[1], values[2], values[3]);

		sleep(1);
	}

	close(sockfd);
}


void *setSpeed(void *arg) {
	int valueEntries[NUM_TRAINS];

	// SOCKET
	int server_sockfd;
	size_t server_len;
	socklen_t client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	
	struct ip_mreq mreq;

	if ( (server_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		printf("An error has occurred on opening socket");
		exit(1);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT_SCKT_SERVER);

	server_len = sizeof(server_address);

	if ( bind(server_sockfd, (struct sockaddr *) &server_address, server_len) < 0 ) {
		perror("An error has occurend on binding");
		exit(1);
	}

	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if ( setsockopt(server_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq) ) < 0) {
		perror("setsockopt");
		exit(1);
	}

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
	BlackGPIO Trem1_T2(GPIO_26, output);
	BlackGPIO Trem1_T3(GPIO_44, output);
	
	while (true) {
		sleepTime = sleep_time[0];

		Trem1_T1.setValue(high);
		sleep(sleepTime);
		Trem1_T1.setValue(low);
		pthread_mutex_lock(&M1);
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
	
	BlackGPIO Trem2_T4(GPIO_69, output);
	BlackGPIO Trem2_T5(GPIO_68, output);
	BlackGPIO Trem2_T6(GPIO_67, output);
	BlackGPIO Trem2_T7(GPIO_60, output);

	while (true) {
		sleepTime = sleep_time[1];

		pthread_mutex_lock(&M1);
		Trem2_T7.setValue(high);
		sleep(sleepTime);
		Trem2_T7.setValue(low);
		pthread_mutex_unlock(&M1);
		Trem2_T4.setValue(high);
		sleep(sleepTime);
		Trem2_T4.setValue(low);
		pthread_mutex_lock(&M2);
		Trem2_T5.setValue(high);
		sleep(sleepTime);
		pthread_mutex_unlock(&M2);
		pthread_mutex_lock(&M4);
		Trem2_T6.setValue(high);
		sleep(sleepTime);
		Trem2_T6.setValue(low);
		pthread_mutex_unlock(&M4);	
	}
	pthread_exit(0);
}

void *trem3(void *arg) {
	int sleepTime = MAX_SLEEP_TIME;

	BlackGPIO Trem3_T8(GPIO_27, output);
	BlackGPIO Trem3_T9(GPIO_47, output);
	BlackGPIO Trem3_T10(GPIO_45, output);
	
	while (true) {
		sleepTime = sleep_time[2];
	
		pthread_mutex_lock(&M2);
		Trem3_T10.setValue(high);
		sleep(sleepTime);
		Trem3_T10.setValue(low);
		pthread_mutex_unlock(&M2);
		Trem3_T8.setValue(high);
		sleep(sleepTime);
		Trem3_T8.setValue(low);
		pthread_mutex_lock(&M3);
		Trem3_T9.setValue(high);
		sleep(sleepTime);
		Trem3_T9.setValue(low);
		pthread_mutex_unlock(&M3);	
	}
	pthread_exit(0);
}

void *trem4(void *arg) {
	int sleepTime = MAX_SLEEP_TIME;

	BlackGPIO Trem4_T11(GPIO_20, output);
	BlackGPIO Trem4_T12(GPIO_49, output);
	BlackGPIO Trem4_T13(GPIO_48, output);
	BlackGPIO Trem4_T14(GPIO_46, output);

	while (true) {
		sleepTime = sleep_time[3];

		Trem4_T11.setValue(high);
		sleep(sleepTime);
		Trem4_T11.setValue(low);
		pthread_mutex_lock(&M5);
		Trem4_T12.setValue(high);
		sleep(sleepTime);
		Trem4_T12.setValue(low);
		pthread_mutex_unlock(&M5);
		pthread_mutex_lock(&M4);
		Trem4_T13.setValue(high);
		sleep(sleepTime);
		Trem4_T13.setValue(low);
		pthread_mutex_unlock(&M4);
		pthread_mutex_lock(&M3);
		Trem4_T14.setValue(high);
		sleep(sleepTime);
		Trem4_T14.setValue(low);
		pthread_mutex_unlock(&M3);

	}
	pthread_exit(0);
}
