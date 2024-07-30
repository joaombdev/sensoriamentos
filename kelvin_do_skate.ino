#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>

// Defina suas credenciais Wi-Fi e Firebase
#define WIFI_SSID "Casa"
#define WIFI_PASSWORD "Rebecca2006"

#define FIREBASE_HOST "ldr-2leds-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "5MCZ3qlKEx31cOUzv262RSIfSz7dEeuAssoYIcHk"

// Define as entradas e saídas na placa
#define LDR_PIN 2
#define LED_GREEN 27
#define LED_RED 26

// Criação de valores âncoras
int ultimoValorLDR = -1;
int diferenca = 100;

// Inicializar o Firebase Data object
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup() {
  Serial.begin(115200);
  
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Desliga todos os leds
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  // Conectar ao WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando-se ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectado ao WiFi: ");
  Serial.println(WiFi.localIP());

  // Configurar o Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.api_key = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // var para a leitura do LDR
  int valorLDR = analogRead(LDR_PIN);

  if (abs(valorLDR - ultimoValorLDR) >= diferenca) {
    // atualiza o novo último valor do LDR
    ultimoValorLDR = valorLDR;

    Serial.println("Mudança");
    Serial.print("LDR: ");
    Serial.println(valorLDR);

    bool led1Status;
    bool led2Status;

    // Verifica se o ambiente está claro ou escuro
    if (valorLDR < 150) {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      Serial.println("ambiente escuro, luz vermelha");
      led1Status = false;
      led2Status = true;
    } else {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      Serial.println("ambiente claro, luz verde");
      led1Status = true;
      led2Status = false;
    }

    // Enviar dados ao Firebase
    if (Firebase.pushInt(firebaseData, "/caixa/ldr", valorLDR)) {
      Serial.println("Dados enviados ao Firebase: LDR");
    } else {
      Serial.print("Erro ao enviar dados ao Firebase: ");
      Serial.println(firebaseData.errorReason());
    }

    // Enviar status do LED1 (verde) ao Firebase
    if (Firebase.setBool(firebaseData, "/caixa/led1", led1Status)) {
      Serial.println("Dados enviados ao Firebase: LED1");
    } else {
      Serial.print("Erro ao enviar dados ao Firebase: ");
      Serial.println(firebaseData.errorReason());
    }

    // Enviar status do LED2 (vermelho) ao Firebase
    if (Firebase.setBool(firebaseData, "/caixa/led2", led2Status)) {
      Serial.println("Dados enviados ao Firebase: LED2");
    } else {
      Serial.print("Erro ao enviar dados ao Firebase: ");
      Serial.println(firebaseData.errorReason());
    }

    Serial.println("");
  }

  delay(500);
}