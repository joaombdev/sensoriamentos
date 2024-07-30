// Link do Firebase
// https://ldr-2leds-default-rtdb.firebaseio.com/

// Bibliotecas
#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "Casa"
#define WIFI_PASSWORD "Rebecca2006"
#define DATABASE_URL "https://ldr-2leds-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "5MCZ3qlKEx31cOUzv262RSIfSz7dEeuAssoYIcHk"

#define LDR_PIN 2 // Mudando para um pino ADC padrão
// Light dependent resistor
#define LED1_PIN 27
// LED verde
#define LED2_PIN 26
// LED vermelho

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int ldrData = 0;
float voltage = 0.0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando na rede");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado com IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Inicializar o ADC
  analogReadResolution(12); // Definir resolução de leitura do ADC (pode variar dependendo da placa)
  analogSetPinAttenuation(LDR_PIN, ADC_11db); // Definir atenuação do ADC para 11dB (máxima faixa de tensão)

  // Definir pinos dos LEDs como saída
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Definir pino do LDR como entrada
  pinMode(LDR_PIN, INPUT);

  // Inicializa os LEDs como desligados
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  int ldrData = analogRead(LDR_PIN);
  voltage = (float)ldrData * 3.3 / 4095; // Calcular a tensão com base na leitura do ADC

  Serial.print("LDR Data: ");
  Serial.println(ldrData);
  Serial.print("Voltagem: ");
  Serial.println(voltage);

  // Determina se o ambiente está claro ou escuro
  if (ldrData < 150) {
    digitalWrite(LED1_PIN, LOW); // Liga o LED1 (ambiente escuro)
    digitalWrite(LED2_PIN, HIGH); // Desliga o LED2
    Serial.println("Ambiente Escuro");
  } else {
    digitalWrite(LED1_PIN, HIGH); // Desliga o LED1
    digitalWrite(LED2_PIN, LOW); // Liga o LED2 (ambiente claro)
    Serial.println("Ambiente Claro");
  }

  // Enviar dados ao Firebase a cada 5 segundos
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Enviar dados ao Firebase
    if (Firebase.setInt(fbdo, "/caixa/luz", ldrData)) {
      Serial.println();
      Serial.print(ldrData);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FALHA: " + fbdo.errorReason());
    }

    if (Firebase.setFloat(fbdo, "/caixa/voltagem", voltage)) {
      Serial.println();
      Serial.print(voltage);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FALHA: " + fbdo.errorReason());
    }
  }

  Serial.println("---------------//---------------");
  delay(1000);
}
