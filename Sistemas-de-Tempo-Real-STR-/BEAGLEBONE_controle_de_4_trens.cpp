#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"
#include <unistd.h>
#include <vector>
#include <array>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

using namespace BlackLib;

int tempo_T1 = 0;
int tempo_T2 = 0;
int tempo_T3 = 0;
int tempo_T4 = 0;

ADC vel_T1(AIN0);
ADC vel_T2(AIN1);
ADC vel_T3(AIN2);
ADC vel_T4(AIN3);

pthread_mutex_t M1, M2, M3, M4, M5;

BlackGPIO Trem1_T1(GPIO_66, output);
BlackGPIO Trem1_T2(GPIO_26, output);
BlackGPIO Trem1_T3(GPIO_44, output);
BlackGPIO Trem2_T4(GPIO_69, output);
BlackGPIO Trem2_T5(GPIO_68, output);
BlackGPIO Trem2_T6(GPIO_67, output);
BlackGPIO Trem2_T7(GPIO_60, output);
BlackGPIO Trem3_T8(GPIO_27, output);
BlackGPIO Trem3_T9(GPIO_47, output);
BlackGPIO Trem3_T10(GPIO_45, output);
BlackGPIO Trem4_T11(GPIO_20, output);
BlackGPIO Trem4_T12(GPIO_49, output);
BlackGPIO Trem4_T13(GPIO_48, output);
BlackGPIO Trem4_T14(GPIO_46, output);

void *trem1(void *arg){
  while(true){
	tempo_T1 = (vel_T1.getIntValue()*10)/1023;	

	Trem1_T1.setValue(high);
	sleep(tempo_T1);
	Trem1_T1.setValue(low);
	pthread_mutex_lock(&M1);
	Trem1_T2.setValue(high);
	sleep(tempo_T1);
	Trem1_T2.setValue(low);
	pthread_mutex_unlock(&M1);
	pthread_mutex_lock(&M5);
	Trem1_T3.setValue(high);
	sleep(tempo_T1);
	Trem1_T3.setValue(low);
	pthread_mutex_unlock(&M5);
  }
  pthread_exit(0);
}

void *trem2(void *arg){
  while(true){
	tempo_T2 = (vel_T2.getIntValue()*10)/1023;	

	pthread_mutex_lock(&M1);
	Trem2_T7.setValue(high);
	sleep(tempo_T2);
	Trem2_T7.setValue(low);
	pthread_mutex_unlock(&M1);
	Trem2_T4.setValue(high);
	sleep(tempo_T2);
	Trem2_T4.setValue(low);
	pthread_mutex_lock(&M2);
	Trem2_T5.setValue(high);
	sleep(tempo_T2);
	pthread_mutex_unlock(&M2);
	pthread_mutex_lock(&M4);
	Trem2_T6.setValue(high);
	sleep(tempo_T2);
	Trem2_T6.setValue(low);
	pthread_mutex_unlock(&M4);
 
  }
  pthread_exit(0);
}

void *trem3(void *arg){
  while(true){
	tempo_T3 = (vel_T3.getIntValue()*10)/1023;	
	
	pthread_mutex_lock(&M2);
	Trem3_T10.setValue(high);
	sleep(tempo_T3);
	Trem3_T10.setValue(low);
	pthread_mutex_unlock(&M2);
	Trem3_T8.setValue(high);
	sleep(tempo_T3);
	Trem3_T8.setValue(low);
	pthread_mutex_lock(&M3);
	Trem3_T9.setValue(high);
	sleep(tempo_T3);
	Trem3_T9.setValue(low);
	pthread_mutex_unlock(&M3);
 
  }
  pthread_exit(0);
}

void *trem4(void *arg){
  while(true){
	tempo_T4 = (vel_T4.getIntValue()*10)/1023;

	Trem4_T11.setValue(high);
	sleep(tempo_T4);
	Trem4_T11.setValue(low);
	pthread_mutex_lock(&M5);
	Trem4_T12.setValue(high);
	sleep(tempo_T4);
	Trem4_T12.setValue(low);
	pthread_mutex_unlock(&M5);
	pthread_mutex_lock(&M4);
	Trem4_T13.setValue(high);
	sleep(tempo_T4);
	Trem4_T13.setValue(low);
	pthread_mutex_unlock(&M4);
	pthread_mutex_lock(&M3);
	Trem4_T14.setValue(high);
	sleep(tempo_T4);
	Trem4_T14.setValue(low);
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

