// Bibliotecas nescessarias
#include <Arduino.h>
//  Teste codigo
#include <time.h>
#include <iostream>
// -----------
#include "WiFi.h"
#include "PubSubClient.h"
#define PIN_LED 25 // Pino usado no ESP-32

#define TOPICO_SUBSCRIBE_LED "topico_liga_desliga_led" // Nome do Topico, pode ser qualquer nome
#define ID_MQTT "esp32_mqtt" // ID do seu MQTT

using namespace std;

const char* SSID = "Nome_Wifi";
const char* PASSWORD = "Senha";

const char* BROKER_MQTT = "IP_Broker";
int BROKER_PORT = 1883;//porta_do_broker
   
WiFiClient espClient;
PubSubClient MQTT(espClient);

//Declarando funções
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);

/* Função: inicializa e conecta-se na rede WI-FI desejada */
void initWiFi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
      
    reconnectWiFi();
}
 
/* Função: inicializa parâmetros de conexão MQTT(endereço do broker, porta e seta função de callback) */
void initMQTT(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback); //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

/* Função: função de callback */
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;

    time_t timer;
    struct tm *timeinfo;

    time(&timer);
    timeinfo = localtime(&timer);

    int hora = timeinfo->tm_hour;
    int minutos = timeinfo->tm_min;

    /* obtem a string do payload recebido */
    for(int i = 0; i < length; i++)
    {
       char c = (char)payload[i];
       msg += c;
    }
 

    Serial.print("Chegou a seguinte mensagem via MQTT: ");
    Serial.println(msg);
    Serial.print("\n");
    
    if (msg.equals("1")) {
        digitalWrite(PIN_LED, HIGH);
        Serial.println("LED aceso mediante comando MQTT \n");
        delay(9000);
        digitalWrite(PIN_LED, LOW);
        Serial.println("LED apagou, passou 9 segundos \n");
    };
    
    if ((hora==7 && minutos==0) || (hora==19 && minutos==37)) {
        digitalWrite(PIN_LED, HIGH);
        Serial.println("acendeu pq deu a hora");
        delay(9000);
        digitalWrite(PIN_LED, LOW);
    };
}
 
/* Função: reconecta-se ao broker MQTT */
void reconnectMQTT(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE_LED); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}

/* Função: verifica o estado das conexões WiFI e ao broker MQTT. */
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT();
      
     reconnectWiFi();
}
 
/* Função: reconecta-se ao WiFi */
void reconnectWiFi(void) 
{
    if (WiFi.status() == WL_CONNECTED)
        return;
          
    WiFi.begin(SSID, PASSWORD); 
      
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}


//----------------------
void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED,LOW);
 
  initWiFi();

  initMQTT();
}

void loop() {
    VerificaConexoesWiFIEMQTT();

    MQTT.loop();
    delay(2000);
}