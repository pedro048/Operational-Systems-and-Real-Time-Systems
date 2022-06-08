#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <array>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>



pthread_mutex_t M1, M2; 



void L(int trem, int trilho, int sono){
	printf("sou o trem %i, estou no trilho %i \n", trem, trilho);
	sleep(sono);
}



void *trem1(void *arg){
  while(true){
	L(1, 1, 1);
  	L(1, 2, 1);
	pthread_mutex_lock(&M1);
	L(1, 3, 10);
	pthread_mutex_unlock(&M2);
	L(1, 4, 1);
  }
  pthread_exit(0);
}

void *trem2(void *arg){
  while(true){
   	L(2, 5, 1);
  	L(2, 6, 1);
	pthread_mutex_lock(&M2);
	L(2, 7, 1);
	pthread_mutex_unlock(&M1);
	L(2, 8, 1);
        
  }
  pthread_exit(0);
}


int main(int argc, char * argv[]) {
    

  int res;
  pthread_t thread1, thread2;

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
    perror("Iniciação do Mutex M1 falhou");
    exit(EXIT_FAILURE);
  }
  pthread_mutex_lock(&M2);

  //------ Thread 1 (executa a fn: trem 1) ------
  res = pthread_create(&thread1, NULL, trem1, NULL);
  if (res != 0)
  {
    perror("Criação da thread 1 falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 2 (executa a fn: trem 2) ------
  res = pthread_create(&thread2, NULL, trem2, NULL);
  if (res != 0)
  {
    perror("Criação da thread 2 falhou");
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


  //----- destruíndo mutex
  pthread_mutex_destroy(&M1);
  pthread_mutex_destroy(&M2);
  exit(EXIT_SUCCESS);

  return 0;
}

