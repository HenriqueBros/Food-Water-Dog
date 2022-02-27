# Food-Water-Dog
Tenho como objetivo desse projeto automatizar a água e ração do meu cachorro, utilizando ESP32 com a linguagem C++, para o controle da válvula solenoide da água e o motor de acionamento da ração.

A comunicação se dará por um servidor MQTT, o mosquitto, que estará hospedado em docker no raspberry pi 3 B+, que funcionará tanto com a hora que será marcada para o funcionamento e ativação dos componentes, quanto no aplicativo, para ser ativado a qualquer momento. O ESP32 estará conectado com a internet via wifi.
