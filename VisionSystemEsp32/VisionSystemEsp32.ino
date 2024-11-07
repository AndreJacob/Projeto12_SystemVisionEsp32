#include <WiFi.h>
#include <PubSubClient.h>

// Defina as credenciais da sua rede Wi-Fi
const char* ssid = "Cobertura";  // Substitua com o nome da sua rede Wi-Fi
const char* password = "Dev@!2024";  // Substitua com a senha da sua rede Wi-Fi

// Defina o broker MQTT
const char* mqtt_server = "test.mosquitto.org";  // Pode ser outro broker MQTT
const int mqtt_port = 1883;  // Porta padrão do MQTT sem TLS
const char* mqtt_topic = "jacob273gato";  // Tópico ao qual o ESP32 está inscrito

// Defina os pinos dos LEDs
const int ledPinD25 = 25;
const int ledPinD26 = 26;
const int ledPinD27 = 27;
const int ledPinD33 = 33;

WiFiClient espClient;
PubSubClient client(espClient);

// Função para conectar ao Wi-Fi
void setup_wifi() {
  delay(10);
  // Conectando-se à rede Wi-Fi
  Serial.println();
  Serial.print("Conectando-se à rede Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao Wi-Fi");
}

// Função para controlar os LEDs de acordo com a mensagem recebida
void controlLEDs(int value) {
  // Desliga todos os LEDs
  digitalWrite(ledPinD25, LOW);
  digitalWrite(ledPinD26, LOW);
  digitalWrite(ledPinD27, LOW);
  digitalWrite(ledPinD33, LOW);

  // Controla os LEDs conforme o valor recebido
  if (value == 1) {
    digitalWrite(ledPinD27, HIGH);  // Liga o LED na porta D27
  } else if (value == 2) {
    digitalWrite(ledPinD26, HIGH);  // Liga o LED na porta D26
  } else if (value == 3) {
    digitalWrite(ledPinD25, HIGH);  // Liga o LED na porta D25
  } else if (value == 4) {
    digitalWrite(ledPinD33, HIGH);  // Liga o LED na porta D33
  }
}

// Função de callback para processar mensagens recebidas no tópico
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  // Converte os bytes da mensagem para string e imprime
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Conteúdo: ");
  Serial.println(message);  // Imprime a mensagem recebida

  // Converte a mensagem para um valor inteiro e controla os LEDs
  int value = message.toInt();  // Converte a mensagem em string para inteiro
  controlLEDs(value);           // Chama a função para controlar os LEDs
}

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Inicializa os pinos dos LEDs como saída
  pinMode(ledPinD25, OUTPUT);
  pinMode(ledPinD26, OUTPUT);
  pinMode(ledPinD27, OUTPUT);
  pinMode(ledPinD33, OUTPUT);

  // Conecta ao Wi-Fi
  setup_wifi();

  // Configura o cliente MQTT e a função de callback
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // Verifica a conexão MQTT
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("Conectando ao broker MQTT...");
      if (client.connect("ESP32_Client")) {
        Serial.println("Conectado!");
        client.subscribe(mqtt_topic);  // Inscreve-se no tópico
      } else {
        Serial.print("Falha na conexão, tentando novamente...");
        delay(5000);
      }
    }
  }
  
  // Processa as mensagens recebidas
  client.loop();
}
