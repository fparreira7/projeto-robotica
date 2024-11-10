#include <Wire.h>

void setup() {

  pinMode(5, OUTPUT);   // Ponte H B-IA
  pinMode(6, OUTPUT);   // Ponte H B-IB
  pinMode(9, OUTPUT);   // Ponte H A-IA
  pinMode(10, OUTPUT);  // Ponte H A-IB


  Wire.begin(8);  // Endereço do Arduino como escravo
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Iniciando");
}

void loop() {
  // Loop vazio, pois o Arduino responde ao evento I2C
}

void receiveEvent(int bytes) {
  int command = Wire.read();  // Lê o comando enviado pelo ESP8266
  switch (command) {
    case 1:
      // Comando para mover para frente
      Serial.println("Movendo para frente");
      frente();
      break;
    case 2:
      // Comando para mover para trás
      Serial.println("Movendo para trás");
      re();
      break;
    case 3:
      // Comando para girar para esquerda
      Serial.println("Girando para a esquerda");
      esquerda();
      break;
    case 4:
      // Comando para girar para direita
      Serial.println("Girando para a direita");
      direita();
      break;
    case 5:
      // Comando para parar
      Serial.println("Parando");
      stop();
      break;
  }
}
void frente() {
  analogWrite(5, 0);
  analogWrite(6, 255);
  analogWrite(9, 0);
  analogWrite(10, 200);
}

void re() {
  analogWrite(5, 250);
  analogWrite(6, 0);
  analogWrite(9, 230);
  analogWrite(10, 0);
}

void esquerda() {
  analogWrite(5, 200);
  analogWrite(6, 0);
  analogWrite(9, 0);
  analogWrite(10, 200);
}

void direita() {
  analogWrite(5, 0);
  analogWrite(6, 200);
  analogWrite(9, 200);
  analogWrite(10, 0);
}

void atacar() {
  analogWrite(5, 200);
  analogWrite(6, 0);
  analogWrite(9, 200);
  analogWrite(10, 0);
}

void stop() {
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, 0);
  analogWrite(10, 0);
}
