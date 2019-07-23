//*************************************//
//        ** ANTES DE RODAR **         //
// 1.Adicione a placa WEMOS D1 a IDE   //
//*************************************//

#include <ESP8266WiFi.h>
//CONFIGURAÇÕES DE WIFI, SERVIDOR, E CONTROLES BÁSICOS
const char* ssid = "NAO FUNCIONA";
const char* password = "@thaty42@";
int ledPin = LED_BUILTIN;
WiFiServer server(80);

void setup() {
  //HABILITA PORTA SERIAL
  Serial.begin(9600);
  delay(10); 
  //DEFINE LEDPIN COMO SAÍDA E DEFAULT DESLIGADO
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  //CRIA E SOBRE SERVIDOR HTTP
  Serial.println((String)"Conectando a rede " + ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado!");
  server.begin();
  //RETORNA DETALHES DA CONEXAO
  Serial.print("Servidor operando em http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  //ESCUTA SERVIDOR
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("Requisição recebida!");
  while(!client.available()){
    delay(1);
  }
  
  //CONFIGURAÇÃO DAS REQUISIÇÕES
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  int value = LOW;
  if (request.indexOf("/PINO13=LIGADO") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/PINO13=DESLIGADO") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  
  //RESPOSTA DO SERVIDOR HTML
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<div>");
  client.print("<h2>Controle do pino 13. </h2>");
  if(value == HIGH) {
    client.print("Status: <strong style='color: green'>LIGADO</strong>");
  } else {
    client.print("Status: <strong style='color: red'>DESLIGADO</strong>");
  }
  client.println("<br>");
  client.println("<a href=\"/PINO13=LIGADO\">Acionar</a><br>");
  client.println("<a href=\"/PINO13=DESLIGADO\">Desacionar</a><br>");
  client.println("</div>");
  client.println("</html>");
  delay(1);
  Serial.println("");
}
