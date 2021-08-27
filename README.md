# Food-Water-Dog
Tenho como objetivo desse projeto automatizar a água e ração do meu cachorro, utilizando arduino com a linguagem C++, para o controle da válvula solenoide da agua e o motor para o torque no eixo do cano de alimentação.

A comunicação se dera por um servidor MQTT, o mosquitto, que estará hospedado em docker no raspberry pi 3 B+, que funcionara tanto com a hora que será marcada para o funcionamento e ativação dos componentes, quanto no aplicativo, para poder ser ativado a qualquer momento. O arduino estará conectado com a internet via wifi.

​	

