// Bibliotecas
#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "iPhone de Igor"
#define WIFI_PASSWORD "igor0305"
#define DATABASE_URL "https://bartolomeu84rt-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "zsZvOqe8Bz1ozl19Mag0jaGzqwogpoSMSnWZkpls"
#define API_KEY "AIzaSyCCmlb-EUSyys7BiBkO2P6bBNNw-5t-ECw"

#define LDR_PIN 2 // Mudando para um pino ADC padrão
#define LED1_PIN 14 // LED verde
#define LED2_PIN 12 // LED vermelho

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0;
float voltage = 0.0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando na rede");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("desconectado - ");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado com IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("SignUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Inicializar o ADC
  // analogReadResolution(12); // Definir resolução de leitura do ADC (pode variar dependendo da placa)
  // analogSetPinAttenuation(LDR_PIN, ADC_11db); // Definir atenuação do ADC para 11dB (máxima faixa de tensão)

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
  ldrData = analogRead(LDR_PIN);
  voltage = (float)ldrData * 3.3 / 4095; // Calcular a tensão com base na leitura do ADC

  Serial.print("LDR Data: ");
  Serial.println(ldrData);
  Serial.print("Voltagem: ");
  Serial.println(voltage);
  Serial.println("---\\---");

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // guardar as info no firebase

    if (Firebase.setInt(fbdo, "/caixa/luz", ldrData)){
      Serial.println();
      Serial.print(ldrData);
      Serial.println(" - salvo em : " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("ERRO - motivo: " + fbdo.errorReason());
    }
  }

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

  Serial.println("---------------//---------------");
  delay(1000);
}
