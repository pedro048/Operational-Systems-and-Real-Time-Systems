#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <array>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

/*
Projeto: Implementação do problema do jantar dos filosofos
com o macanismo mutex.
Objetivo: Resolver os deadlocks
Autor: Pedro Victor Andrade Alves

	F1
F5		F2
    F4     F3

    h5     h1
		h2
  h4	h3


*/

pthread_mutex_t h1, h2, h3, h4, h5; // os mutex fazem referencia aos hashis

void FP(int filosofo, int temp_pensar){
	printf("sou o filosofo %i, estou pensando\n", filosofo);
	sleep(temp_pensar);
}

void FC(int filosofo, int temp_comer){
	printf("sou o filosofo %i, estou comendo\n", filosofo);
	sleep(temp_comer);
}


void *filosofo1(void *arg){
  while(true){
	FP(1, 1);
	pthread_mutex_lock(&h1);
	sleep(1);
  	pthread_mutex_lock(&h5);
        FC(1, 1);
	pthread_mutex_unlock(&h1);
	pthread_mutex_unlock(&h5);
  }
  pthread_exit(0);
}

void *filosofo2(void *arg){
  while(true){
	FP(2, 1);
	pthread_mutex_lock(&h1);
	sleep(1);
  	pthread_mutex_lock(&h2);
        FC(2, 1);
	pthread_mutex_unlock(&h1);
	pthread_mutex_unlock(&h2);
  }
  pthread_exit(0);
}

void *filosofo3(void *arg){
  while(true){
	FP(3, 1);
	pthread_mutex_lock(&h3);
	sleep(1);
  	pthread_mutex_lock(&h2);
        FC(3, 1);
	pthread_mutex_unlock(&h3);
	pthread_mutex_unlock(&h2);
  }
  pthread_exit(0);
}

void *filosofo4(void *arg){
  while(true){
	FP(4, 1);
	pthread_mutex_lock(&h3);
	sleep(1);
  	pthread_mutex_lock(&h4);
        FC(4, 1);
	pthread_mutex_unlock(&h3);
	pthread_mutex_unlock(&h4);
  }
  pthread_exit(0);
}

void *filosofo5(void *arg){
  while(true){
	FP(5, 1);
	pthread_mutex_lock(&h5);
	sleep(1);
  	pthread_mutex_lock(&h4);
        FC(5, 1);
	pthread_mutex_unlock(&h5);
	pthread_mutex_unlock(&h4);
  }
  pthread_exit(0);
}

int main(int argc, char * argv[]) {
    

  int res;
  pthread_t thread1, thread2, thread3, thread4, thread5;

  void *thread_result;
  

  // ------ criando multex h1 ------
  res = pthread_mutex_init(&h1, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h1 falhou");
    exit(EXIT_FAILURE);
  }
 
   // ------ criando multex h2 ------
  res = pthread_mutex_init(&h2, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h2 falhou");
    exit(EXIT_FAILURE);
  }
  
     // ------ criando multex h3 ------
  res = pthread_mutex_init(&h3, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h3 falhou");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex h4 ------
  res = pthread_mutex_init(&h4, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h4 falhou");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex h5 ------
  res = pthread_mutex_init(&h5, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h5 falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 1 (executa a fn: filosofo 1) ------
  res = pthread_create(&thread1, NULL, filosofo1, NULL);
  if (res != 0)
  {
    perror("Criação da thread 1 falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 2 (executa a fn: filosofo 2) ------
  res = pthread_create(&thread2, NULL, filosofo2, NULL);
  if (res != 0)
  {
    perror("Criação da thread 2 falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 3 (executa a fn: filosofo 3) ------
  res = pthread_create(&thread3, NULL, filosofo3, NULL);
  if (res != 0)
  {
    perror("Criação da thread 3 falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 4 (executa a fn: filosofo 4) ------
  res = pthread_create(&thread4, NULL, filosofo4, NULL);
  if (res != 0)
  {
    perror("Criação da thread 4 falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 5 (executa a fn: filosofo 5) ------
  res = pthread_create(&thread5, NULL, filosofo5, NULL);
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
  pthread_mutex_destroy(&h1);
  pthread_mutex_destroy(&h2);
  pthread_mutex_destroy(&h3);
  pthread_mutex_destroy(&h4);
  pthread_mutex_destroy(&h5);
  exit(EXIT_SUCCESS);

  return 0;
}
