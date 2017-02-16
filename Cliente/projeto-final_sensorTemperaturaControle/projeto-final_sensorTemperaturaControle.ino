/*
  Hardware Livre USP - Mastertech
  Solicitando página web, enviando dado via get
*/
#include <SPI.h>
#include <Ethernet.h>

// Utilize o endereço MAC que está na etiqueta branca da
// sua Galielo
byte mac[] = { 0x98, 0x4F, 0xEE, 0x05, 0xAB, 0x8E }; 

// IP do servidor e porta do servidor.
//IPAddress server(192,168,1,32);
char server[] = "0.tcp.ngrok.io";
int port = 10167;


//int tolerancia = 21;
String total = "";


EthernetClient client;

// Variaveis para delay de envio de informacoes
const long interval = 5000;
unsigned long previousMillis = 0;


void setup() {
  Serial.begin(9600);
  system("ifup eth0");
  Serial.println("Tentando obter um IP:");
  while (!Ethernet.begin(mac)) {
    Serial.println("Erro ao conectar");
  }
  Serial.print("Meu endereco:");
  Serial.println(Ethernet.localIP());

  // Para dar tempo de abrir o terminal
  analogRead(A0);
  delay(3000);

  total.reserve(100);
}

void loop(){
  // Mostrar toda a resposta
  if (client.available()) {
    while(client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    client.stop();
  }

  // Se o servidor desconectar
  if (!client.connected()) {
    float data, soma;
    soma = 0;
    for(int i = 0;i < 20; i++) {
      data = analogRead(A0);
      data *= 100.0*5.0/1024;
      soma = soma + data;
      delay(20); 
    }

    data = soma/20.0-9.0;
    Serial.println(data);

    char msg[100];
    sprintf(msg, "{\"temp\":\"%.2f\",\"id\":\"sala01\"}", data);
    total =  String(msg);

    int tamanho;

    tamanho = total.length();
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (client.connect(server, port)) {
        Serial.println("Conectado ao servidor\n\n");
        client.print("GET /in/");
        client.print(total);
        Serial.println(total);
        client.println(" HTTP/1.0");
        client.println("Host: 0.tcp.ngrok.io");
        client.println();
      } else {
        // Problema na conexão
        Serial.println("connection failed");
      }
    }
    
   

  }
}


