#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

const char* ssid = "SUAREDE";
const char* senha = "SENHA";
const char* servidor_mqtt = "SERVERMQTT";

unsigned long tempoAnterior = 0;
const long intervalo = 7000;

WiFiClient clienteWiFi;
PubSubClient clienteMQTT(clienteWiFi);

const int pinoUmidade = 34;   // Pino para o potenciômetro de umidade
const int pinoTemperatura = 35; // Pino para o potenciômetro de temperatura
const int pinoLedRemoto = 19;  // Pino D19 para LED de controle remoto
const int pinoLedStatus = 18;  // Pino D18 para LED de status

int umidade_mqtt = 0;
int status = 0;
int remoto = 0;

void callbackMQTT(char* topico, byte* payload, unsigned int tamanho) {
  String mensagem = "";
  for (int i = 0; i < tamanho; i++) {
    mensagem += (char)payload[i];
  }

  if (String(topico) == "umidade") {
    umidade_mqtt = mensagem.toInt();
    Serial.print("Umidade recebida do MQTT: ");
    Serial.println(umidade_mqtt);
  }

  if (String(topico) == "remoto") {
    remoto = mensagem.toInt();
    Serial.print("Valor do tópico remoto recebido: ");
    Serial.println(remoto);
    controlarLedsRemoto();
  }
}

void configurarWifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);

  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconectarMQTT() {
  while (!clienteMQTT.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (clienteMQTT.connect("espClient")) {
      Serial.println("conectado");
      clienteMQTT.subscribe("umidade");
      clienteMQTT.subscribe("remoto");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void controlarLedsRemoto() {
  if (remoto == 1) {
    digitalWrite(pinoLedRemoto, HIGH);
    digitalWrite(pinoLedStatus, HIGH);
    Serial.println("LED remoto ligado (remoto ativado)");
  } else {
    digitalWrite(pinoLedRemoto, LOW);
    digitalWrite(pinoLedStatus, LOW);
    Serial.println("LED remoto desligado (remoto desativado)");
  }
}

void controlarLedStatus() {
  if (status == 1) {
    digitalWrite(pinoLedStatus, HIGH);
  } else {
    digitalWrite(pinoLedStatus, LOW);
  }
}

void publicarDados() {
  int valorUmidade = analogRead(pinoUmidade);
  int valorTemperatura = analogRead(pinoTemperatura);

  int umidadeMapeada = map(valorUmidade, 0, 4095, 0, 100);
  int temperaturaMapeada = map(valorTemperatura, 0, 4095, 0, 100);

  if (remoto == 0) {
    if (umidadeMapeada < umidade_mqtt) {
      status = 1;
    } else if (umidadeMapeada > (umidade_mqtt + 15)) {
      status = 0;
    }
  }

  controlarLedStatus();

  String statusStr = (status == 1) ? "ligado" : "desligado";
  clienteMQTT.publish("agalu3", statusStr.c_str());
  clienteMQTT.publish("agalu", String(umidadeMapeada).c_str());
  clienteMQTT.publish("agalu2", String(temperaturaMapeada).c_str());

  Serial.print("Umidade do Potenciômetro: ");
  Serial.print(umidadeMapeada);
  Serial.print("%, Temperatura: ");
  Serial.print(temperaturaMapeada);
  Serial.print("%, Status: ");
  Serial.println(statusStr);
}

void setup() {
  Serial.begin(115200);
  pinMode(pinoLedRemoto, OUTPUT);
  pinMode(pinoLedStatus, OUTPUT);
  pinMode(2, OUTPUT);

  configurarWifi();
  clienteMQTT.setServer(servidor_mqtt, 1883);
  clienteMQTT.setCallback(callbackMQTT);
}

void loop() {
  unsigned long tempoAtual = millis();
  clienteMQTT.loop();
  reconectarMQTT();

  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;
    publicarDados();
  }
}
