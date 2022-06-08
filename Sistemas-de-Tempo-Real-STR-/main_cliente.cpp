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

void *readEntries(void *arg);

int main(int argc, char * argv[]) {

	int res;
 	pthread_t thread1;

 	void *thread_result;

 	 //------ Thread 1 (executa a fn: readEntries) ------
	  res = pthread_create(&thread1, NULL, readEntries, NULL);
	  if (res != 0)
	  {
	    perror("Criação da thread 1 falhou");
	    exit(EXIT_FAILURE);
	  }

	   // ----- Espera termino das threads
	  res = pthread_join(thread1, &thread_result);
	  if (res != 0)
	  {
	    perror("Juncao da Thread 1 falhou");
	    exit(EXIT_FAILURE);
	  }

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
