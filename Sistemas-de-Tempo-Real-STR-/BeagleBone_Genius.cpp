#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "BlackGPIO/BlackGPIO.h"
#include <unistd.h>
#include <vector>
#include <array>

// Projeto do jogo Genius com a BleagleBone Black. Pedro Victor e Francisco Kennedi 

using namespace BlackLib;

int main(int argc, char * argv[]) {
    clock_t start, end;
    bool statusBotao = false;
    bool nextLevel, debounceStart;
    double intermediate, tempo_jogo = 5, tempo_now;
    int randLed, lastRand = 9999;

    BlackGPIO botaoLed_1(GPIO_26, input);
    BlackGPIO botaoLed_2(GPIO_44, input);
    BlackGPIO botaoLed_3(GPIO_68, input);
    BlackGPIO botaoLed_4(GPIO_67, input);
    BlackGPIO botaoStart(GPIO_60, input);

    BlackGPIO Led_1(GPIO_47, output);
    BlackGPIO Led_2(GPIO_45, output);
    BlackGPIO Led_3(GPIO_49, output);
    BlackGPIO Led_4(GPIO_48, output);
    BlackGPIO Led_vermelho(GPIO_46, output);

    std::vector<int> sequencias_jogo;
    std::vector<int> sequencias_jogador;

    Led_1.setValue(low);
    Led_2.setValue(low);
    Led_3.setValue(low);
    Led_4.setValue(low);
    Led_vermelho.setValue(low);

    srand(time(NULL));

    while (true) {
        std::cout << " #### " << botaoStart.getValue() << std::endl;
        if (botaoStart.getValue() == "1") {
            do {
                Led_1.setValue(low);
                Led_2.setValue(low);
                Led_3.setValue(low);
                Led_4.setValue(low);
    		Led_vermelho.setValue(low);

                sleep(1);

                nextLevel = true;
                randLed = rand() % 4 + 1;

		while (randLed == lastRand) {
                    randLed = rand() % 4 + 1;
                }
                
                lastRand = randLed;
                sequencias_jogo.push_back(randLed);


                for (int i = 0; i < sequencias_jogo.size(); i++) {
			std::cout << randLed << std::endl;
                    switch (sequencias_jogo[i]) {
                        case 1:
                            Led_1.setValue(high);
                            sleep(1);
                            Led_1.setValue(low);
                            break;
                        case 2:
                            Led_2.setValue(high);
                            sleep(1);
                            Led_2.setValue(low);
                            break;
                        case 3:
                            Led_3.setValue(high);
                            sleep(1);
                            Led_3.setValue(low);
                            break;
                        case 4:
                            Led_4.setValue(high);
                            sleep(1);
                            Led_4.setValue(low);

                            break;
                    }
                }

                while (!statusBotao) {
                    if (botaoStart.getValue() == "1") {
                        statusBotao = true;
                        debounceStart = false;
                    }
                }
                
                if(statusBotao) start = clock();
                
                while (statusBotao) {
                    if (botaoStart.getValue() == "1" && debounceStart) statusBotao = false;
                    
		if (sequencias_jogador.size() == 0) {
                    if (botaoLed_1.getValue() == "1") {
                        debounceStart = true;
                        sequencias_jogador.push_back(1);
			
                    }
                    else if (botaoLed_2.getValue() == "1") {
                        debounceStart = true;
                        sequencias_jogador.push_back(2);
			
                    }
                    else if (botaoLed_3.getValue() == "1") {
                        debounceStart = true;
                        sequencias_jogador.push_back(3);
			
                    }
                    else if (botaoLed_4.getValue() == "1") {
                        debounceStart = true;
                        sequencias_jogador.push_back(4);
			
                    }
		}else {
			 if (botaoLed_1.getValue() == "1" && sequencias_jogador.back() != 1) {
                        debounceStart = true;
                        sequencias_jogador.push_back(1);
			
                    }
                    else if (botaoLed_2.getValue() == "1" && sequencias_jogador.back() != 2) {
                        debounceStart = true;
                        sequencias_jogador.push_back(2);
			
                    }
                    else if (botaoLed_3.getValue() == "1" && sequencias_jogador.back() != 3) {
                        debounceStart = true;
                        sequencias_jogador.push_back(3);
			
                    }
                    else if (botaoLed_4.getValue() == "1" && sequencias_jogador.back() != 4) {
                        debounceStart = true;
                        sequencias_jogador.push_back(4);
			
                    }
		}
                    
         
                    tempo_now = tempo_jogo - (double)(clock()-start)/(double)(CLOCKS_PER_SEC);
                    std::cout << "Tempo de jogo: " << tempo_now << std::endl;
                    
                    if (tempo_now <= 0.9) {
                        
                        break;
                    }
                }
                
                end = clock();

                if (sequencias_jogador.size() != sequencias_jogo.size() && nextLevel) {
                    std::cout << "Sequência errada, Fim de jogo!" << std::endl;
                    Led_vermelho.setValue(high);
                    nextLevel = false;                    
                } else {                    
                    for (int i = 0; i < sequencias_jogo.size(); i++)  {

                        if (sequencias_jogo[i] != sequencias_jogador[i]) {
                            std::cout << "Sequência errada, Fim de jogo!" << std::endl;
                            Led_vermelho.setValue(high);
                            nextLevel = false; 
                            break;                           
                        }
                    }
                }

                if (nextLevel) {
                    intermediate = (double)(end-start)/(double)(CLOCKS_PER_SEC);
			tempo_jogo += 0.5;
                    std::cout <<
"----------------------------------------------------------------------- " << std::endl; 
                    std::cout << "Tempo da partida: " << intermediate << std::endl;
                    std::cout << "----------------------------------------------------------------------- " << std::endl; 
                  


                sequencias_jogador.erase(sequencias_jogador.begin(), sequencias_jogador.begin() + sequencias_jogador.size());
		}
            } while (nextLevel); 
            break;
        }
    }

    Led_1.setValue(low);
    Led_2.setValue(low);
    Led_3.setValue(low);
    Led_4.setValue(low);

    sleep(5);
    return 0;
}
