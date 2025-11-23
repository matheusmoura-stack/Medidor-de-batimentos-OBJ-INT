#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define LARGURA_OLED 128
#define ALTURA_OLED 64
#define OLED_RESET -1

Adafruit_SSD1306 tela(LARGURA_OLED, ALTURA_OLED, &Wire, OLED_RESET);


#define PINO_PULSO 35
#define PINO_LED   2
#define PINO_BUZZER 25

int limiteMin = 50;
int limiteMax = 120;


const char* wifiSSID = "Wokwi-GUEST";
const char* wifiPASS = "";
const char* mqttBroker = "test.mosquitto.org";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);


void inicializarWiFi() {
  Serial.println("Iniciando conexao Wi-Fi...");
  WiFi.begin(wifiSSID, wifiPASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("#");
    delay(400);
  }

  Serial.println("\nWi-Fi conectado!");
}


void reconectarMQTT() {
  while (!mqtt.connected()) {
    Serial.println("Conectando ao servidor MQTT...");

    if (mqtt.connect("MonitorCardiaco_ESP32")) {
      Serial.println("Broker MQTT acessado com sucesso!");
    } else {
      Serial.print("Falha! Código: ");
      Serial.println(mqtt.state());
      Serial.println("Tentando novamente em 4 segundos...");
      delay(4000);
    }
  }
}

void tocarAlarme() {
  for (int i = 0; i < 3; i++) {
    tone(PINO_BUZZER, 1200); 
    delay(180);
    noTone(PINO_BUZZER);
    delay(180);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);

  // Iniciar display
  if (!tela.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("ERRO: OLED nao encontrado!");
    while (true);
  }

  tela.clearDisplay();
  tela.setTextSize(1);
  tela.setTextColor(SSD1306_WHITE);
  tela.setCursor(0, 25);
  tela.println("Inicializando sistema...");
  tela.display();
  delay(1500);

  inicializarWiFi();

  mqtt.setServer(mqttBroker, 1883);
}


void loop() {
  if (!mqtt.connected()) {
    reconectarMQTT();
  }
  mqtt.loop();

  // Simulacao de valor vindo do sensor
  int leituraBruta = analogRead(PINO_PULSO);
  int batimento = map(leituraBruta, 300, 600, 40, 180);
  batimento = constrain(batimento, 0, 200);

  String nivel;


  if (batimento < limiteMin) {
    nivel = "baixo";

    tela.clearDisplay();
    tela.setCursor(0, 0);
    tela.println("Frequencia baixa!");
    tela.setCursor(0, 20);
    tela.print("Atual: ");
    tela.print(batimento);
    tela.println(" bpm");
    tela.display();

    digitalWrite(PINO_LED, HIGH);
    tocarAlarme();
  }
  else if (batimento > limiteMax) {
    nivel = "alto";

    tela.clearDisplay();
    tela.setCursor(0, 0);
    tela.println("Frequencia elevada!");
    tela.setCursor(0, 20);
    tela.print("Atual: ");
    tela.print(batimento);
    tela.println(" bpm");
    tela.display();

    digitalWrite(PINO_LED, HIGH);
    tocarAlarme();
  }
  else {
    nivel = "normal";

    tela.clearDisplay();
    tela.setCursor(0, 0);
    tela.println("Frequencia dentro");
    tela.println("dos parametros.");
    tela.setCursor(0, 25);
    tela.print("BPM: ");
    tela.println(batimento);
    tela.display();

    digitalWrite(PINO_LED, LOW);
    noTone(PINO_BUZZER);
  }

  // Logs informativos
  Serial.print("Leitura: ");
  Serial.print(batimento);
  Serial.print(" bpm   | Estado: ");
  Serial.println(nivel);

  char msgBPM[10];
  char msgStatus[20];
  sprintf(msgBPM, "%d", batimento);
  nivel.toCharArray(msgStatus, 20);

  mqtt.publish("monitor/bpm", msgBPM);
  mqtt.publish("monitor/status", msgStatus);

  delay(2000);
}
