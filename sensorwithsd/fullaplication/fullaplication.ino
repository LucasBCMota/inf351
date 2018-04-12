// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Ultrasonic.h>
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define pino_echo 7
#define pino_trigger 6
#include <SPI.h>
#include <SD.h>

File configuracao;
// O arquivo de configuração deve chamar config.txt e conter nas 6 primeiras linhas apenas inteiros
// Indicando na ordem dia, mes, ano, hora, minuto, intervalo. 
//Exemplo do config.txt contendo as configuraçoes default:
//10
//4
//2018
//8
//0
//2000
//qualquer dado apos a 6ª linha é ignorado.


DHT dht(DHTPIN, DHTTYPE);
Ultrasonic ultrasonic(pino_trigger, pino_echo);

int sensorPin = A0;
int sensorValue = 0;
int dia = 10, mes = 4, ano = 2018, hora = 8, minuto = 0, intervalo = 2000;
int inicio = millis();
String nome;
File arquivo;

void setup() {
  Serial.begin(9600);
  dht.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  if (SD.exists("config.txt")) {
    configuracao = SD.open("config.txt");
    dia = (configuracao.readStringUntil('\n')).toInt();
    mes = (configuracao.readStringUntil('\n')).toInt();
    ano = (configuracao.readStringUntil('\n')).toInt();
    hora = (configuracao.readStringUntil('\n')).toInt();
    minuto = (configuracao.readStringUntil('\n')).toInt();
    intervalo = (configuracao.readStringUntil('\n')).toInt();
    configuracao.close();
    Serial.println("Arquivo de configuração carregado");
  } else {
    Serial.println("Arquivo de configuração nao encontrado, usando valores default");
  }
  nome.concat(dia);
  nome.concat("-");
  nome.concat(mes);
  nome.concat("-");
  nome.concat(ano%2000);
  nome.concat(".csv");
  arquivo = SD.open(nome, FILE_WRITE);
  if (arquivo)
    arquivo.println("Leituras dos sensores DHT22, Ultrasonic e LDR;");
  else
    Serial.println("Problemas ao criar arquivo");
  arquivo.close();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(intervalo);
  sensorValue = analogRead(sensorPin);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int deltaTime = millis() - inicio;
  int tempoEmSegundos = deltaTime/1000;
  int deltaHoras = (tempoEmSegundos/3600);
  int deltaMinutos = (tempoEmSegundos -(3600*deltaHoras))/60;
  int deltaDias = deltaHoras/24;
  deltaHoras = deltaHoras%24;
  if(minuto + deltaMinutos > 60){
    deltaHoras++;
    deltaMinutos = deltaMinutos - 60;
  }
  if(hora+deltaHoras > 24){
    deltaDias++;
    deltaHoras = deltaHoras-24;
  }
  arquivo = SD.open(nome, FILE_WRITE);
  arquivo.write(String(dia+deltaDias).c_str());
  arquivo.write("/");
  arquivo.write(String(mes).c_str());
  arquivo.write("/");
  arquivo.write(String(ano).c_str());
  arquivo.write(";");
  arquivo.println();
  arquivo.write(String(hora+deltaHoras).c_str());
  arquivo.write(":");
  arquivo.write(String(minuto+deltaMinutos).c_str());
  arquivo.write(";");
  arquivo.write(String(t).c_str());
  arquivo.write("*C;");
  arquivo.write(String(ultrasonic.distanceRead()).c_str());
  arquivo.write("cm;");
  arquivo.write(String(sensorValue).c_str());
  arquivo.write(';');
  arquivo.println();
  arquivo.close();
}
