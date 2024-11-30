# HortaIFSP
Código e explicação da horta automatizada conectada a uma máquina virtual Azure.

IP máquina: 74.179.84.129

Foi utilizada uma máquina Ubuntu 24.04 que está rodando um broker MQTT, um banco de dados MySQL e um servidor Node-Red.

![Maquina criada na Azure](maquinaazure.jpg)


Implementação do Banco de dados utilizado no projeto:

![Tabelas do Banco de Dados](banco_de_Dados.jpg)



A implementação no Node-red foi feita em etapas dividas.

Abaixo, tem-se os nós que fazem a leitura dos dados escritos em tópicos MQTT (vindo do ESP 32) e salvos para o banco de dados Mysql.

![Nós para leitura de dados MQTT e salvo no BD Mysql](mqttmysql.jpg)


A seguir, implementação das leituras dos status de irrigação para o Dashboard.

![Nós para estado da irrigação no Dashboard](statusdashboard.jpg)


Atuação dos botões de irrigação manual no dashboard, salvos no MySQL. 

![Nós para botões de irrigação manual](remotodashboardbotao.jpg)


Escolha de umidade limite no dashboard e envio para BD MySQL.

![Nós para escolha umidade](escolhaumidadedashboard.jpg)


Nós para escrita de umidade escolhida e irrigação manual para MQTT que será lido pelo ESP 32.
![Nós escrita umiade escolhida e irrigação manual](mysqlmqtt.jpg)


Dashboard criado no servidor Node-Red:

![Dashboard](dashboard.jpg)



A montagem física do sistema está representada abaixo onde potenciômetros foram inseridos para simular sensores e LEDs para simular atuadores.

![Montagem Protoboard](esquema-proto.png)


Figuras de implementações reais dos sensores: 

![Sensor de Temperatura](sensor-temperatura.jpg)

![Sensor de Umidade do Solo](sensorsolo.jpg)


Figuras de implementações reais dos atuadores: 

![Buzzer](buzzer.jpg)

![Válvula Solenóide](valvula.jpg)


