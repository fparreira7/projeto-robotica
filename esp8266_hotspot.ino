#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <Wire.h>

// Configurações do hotspot
const char *ssid = "ROBO-DO-CURE-VISION";
const char *password = "jeff12345";

// Endereço I2C do Arduino
const int arduinoAddress = 8;

// Configuração do servidor web na porta 80
ESP8266WebServer server(80);
DNSServer dnsServer;

// IP do ESP8266 no modo AP
IPAddress apIP(192, 168, 4, 1);

void setup() {
  Serial.begin(115200);

  // Configura o ESP8266 como um ponto de acesso
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  Serial.println("Hotspot criado. Conecte-se e acesse 192.168.4.1");

  // Inicializa o servidor DNS para redirecionar todas as requisições para o ESP8266
  dnsServer.start(53, "*", apIP);

  // Inicializa I2C no ESP8266
  Wire.begin();

  // Configura as rotas do servidor web para cada comando
  server.on("/", handleRoot);
  server.on("/frente", []() { sendCommandToArduino(1); });
  server.on("/tras", []() { sendCommandToArduino(2); });
  server.on("/esquerda", []() { sendCommandToArduino(3); });
  server.on("/direita", []() { sendCommandToArduino(4); });
  server.on("/parar", []() { sendCommandToArduino(5); });

  // Inicia o servidor web
  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  dnsServer.processNextRequest();  // Processa solicitações DNS para o captive portal
  server.handleClient();           // Processa requisições HTTP
}

// Função para enviar comandos via I2C para o Arduino
void sendCommandToArduino(int command) {
  Wire.beginTransmission(arduinoAddress);
  Wire.write(command);  // Envia o comando
  Wire.endTransmission();

  // Responde ao navegador indicando que o comando foi enviado
  server.send(200, "text/plain", "Comando enviado: " + String(command));
}

// Página HTML básica com botões para controle
void handleRoot() {
  String html = 
"<!DOCTYPE html>"
"<html lang=\"pt-BR\">"
"<head>"
"    <meta charset=\"UTF-8\">"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
"    <title>Controle do Robô</title>"
"    <style>"
"        body {"
"            display: flex;"
"            flex-direction: column;"
"            align-items: center;"
"            justify-content: center;"
"            height: 100vh;"
"            background-color: #282c34;"
"            color: white;"
"            font-family: 'Arial', sans-serif;"
"        }"
"        h1 {"
"            margin-bottom: 20px;"
"        }"
"        .controller {"
"            display: flex;"
"            flex-direction: column;"
"            align-items: center;"
"            background-color: #3c4043;"
"            border-radius: 15px;"
"            padding: 20px;"
"            box-shadow: 0 0 20px rgba(0, 0, 0, 0.5);"
"        }"
"        .arrow {"
"            font-size: 3em;"
"            cursor: pointer;"
"            margin: 10px;"
"            user-select: none;"
"            transition: transform 0.2s;"
"        }"
"        .arrow:hover {"
"            transform: scale(1.2);"
"            color: #61dafb;"
"        }"
"        .button-container {"
"            display: flex;"
"            flex-direction: column;"
"            align-items: center;"
"        }"
"        .horizontal {"
"            display: flex;"
"            justify-content: center;"
"        }"
"        .meio {"
"            display: flex;"
"        }"
"    </style>"
"    <script>"
"        function sendCommand(command) {"
"            fetch(`/${command}`, { method: 'POST' })"
"                .then(response => {"
"                    if (!response.ok) {"
"                        console.error('Erro ao enviar comando:', command);"
"                    }"
"                })"
"                .catch(error => {"
"                    console.error('Erro:', error);"
"                });"
"        }"
"    </script>"
"</head>"
"<body>"
"    <h1>Controle do Robô</h1>"
"    <div class=\"controller\">"
"        <div class=\"arrow\" onclick=\"sendCommand('frente')\">&#8593;</div>"
"        <div class=\"meio\">"
"            <div class=\"arrow\" onclick=\"sendCommand('esquerda')\">&#8592;</div>"
"            <div class=\"arrow\" onclick=\"sendCommand('parar')\">⏹️</div>"
"            <div class=\"arrow\" onclick=\"sendCommand('direita')\">&#8594;</div>"
"        </div>"
"        <div class=\"arrow\" onclick=\"sendCommand('tras')\">&#8595;</div>"
"    </div>"
"</body>"
"</html>";
  server.send(200, "text/html", html);
}
