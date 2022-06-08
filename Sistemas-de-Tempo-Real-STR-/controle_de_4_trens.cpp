#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <array>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>


pthread_mutex_t M1, M2, M3, M4, M5;

void L(int trem, int trilho, int tempo){
	printf("sou o trem %i, estou no trilho %i \n", trem, trilho);
	sleep(tempo);
}

void *trem1(void *arg){
  while(true){
	L(1, 1, 1);
	pthread_mutex_lock(&M1);
	L(1, 2, 1);
	pthread_mutex_unlock(&M1);
	pthread_mutex_lock(&M5);
	L(1, 3, 1);
	pthread_mutex_unlock(&M5);
  }
  pthread_exit(0);
}

void *trem2(void *arg){
  while(true){
	pthread_mutex_lock(&M1);
	L(2, 7, 1);
	pthread_mutex_unlock(&M1);
	L(2, 4, 1);
	pthread_mutex_lock(&M2);
	L(2, 5, 1);
	pthread_mutex_unlock(&M2);
	pthread_mutex_lock(&M4);
	L(2, 6, 1);
	pthread_mutex_unlock(&M4);
 
  }
  pthread_exit(0);
}

void *trem3(void *arg){
  while(true){
	pthread_mutex_lock(&M2);
	L(3, 10, 1);
	pthread_mutex_unlock(&M2);
	L(3, 8, 1);
	pthread_mutex_lock(&M3);
	L(3, 9, 1);
	pthread_mutex_unlock(&M3);
 
  }
  pthread_exit(0);
}

void *trem4(void *arg){
  while(true){
	L(4, 11, 1);
	pthread_mutex_lock(&M5);
	L(4, 12, 1);
	pthread_mutex_unlock(&M5);
	pthread_mutex_lock(&M4);
	L(4, 13, 1);
	pthread_mutex_unlock(&M4);
	pthread_mutex_lock(&M3);
	L(4, 14, 1);
	pthread_mutex_unlock(&M3);
 
  }
  pthread_exit(0);
}


int main(int argc, char * argv[]) {
    
  int res;
  pthread_t thread1, thread2, thread3, thread4;

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

  //----- destruíndo mutex
  pthread_mutex_destroy(&M1);
  pthread_mutex_destroy(&M2);
  pthread_mutex_destroy(&M3);
  pthread_mutex_destroy(&M4);
  pthread_mutex_destroy(&M5);
  exit(EXIT_SUCCESS);

  return 0;
}

