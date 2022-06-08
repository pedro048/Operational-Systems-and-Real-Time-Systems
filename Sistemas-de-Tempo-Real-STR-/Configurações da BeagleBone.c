Configurações da BeagleBone:

SEGURANÇA:

- Desligar placa pela linha de comando:

	$ sudo shutdown -h now

- Não utilizar os pinos de voltagem 5V

- Utilizar pinos de voltagem 3.3V (Pinos P9_3 e P9_4)

- Quando utilizar o ADC, utilizar pinos de voltagem e terra próprios (1,8V)

	VDD_ADC: pino P9_32
	GND_ADC: pino P9_34

ACESSANDO A BEAGLEBONE:

	$ ssh debian@192.168.7.2
	senha: temppwd

- Teste de instalação:

	$ cat /etc/dogtag

- Deve instalar a BlackLib na Beagle:

	Para verificar se a biblioteca foi instalada corretamente, execute os seguintes
	comandos:

	$ cd BlackLib-Modificada
	$ make

	Após executar o comando make, o executável para o arquivo main, dentro da lib, é
	criado.	Para testar se deu tudo certo com a biblioteca, execute-o com o seguinte comando:

	./main

	O seguinte texto é exibido: ‘Este é o seu main, implemente-o'

ENVIANDO PROGRAMAS PARA A BEAGLE:

- Instalação do compilador (na sua máquina):

	$ sudo apt-get install g++-arm-linux-gnueabihf

- Verificar versão do compilador:

	$ arm-linux-gnueabihf-g++ --version

- Compilação de um código:

	$ arm-linux-gnueabihf-g++ main.cpp -o main

- Envio para a BeagleBone:

	$ scp main debian@192.168.7.2:/home/debian/

COMPILANDO NA BEAGLE:

- cd beagleBone-Modificada-master
- make clean
- make
-sudo ./main






