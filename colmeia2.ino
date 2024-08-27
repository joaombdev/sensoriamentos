// Biblioteca para o wifi e firebase
#include <WiFi.h>
#include <FirebaseESP32.h>

// Biblioteca para o DHT22 e HX711
#include <DHT.h>
#include <HX711.h>

// Credenciais do wifi
// SSID - Casa
// wifi pwd - Rebecca2006
// SSID - Galaxy Note10+5a96
// wifi pwd - hnds4728
// SSID e senha da rede Wi-Fi
#define WIFI_SSID "Galaxy Note10+5a96"
#define WIFI_PASSWORD "hnds4728"

// Credenciais do firebase
#define DATABASE_URL "https://bartolomeu84rt-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "zsZvOqe8Bz1ozl19Mag0jaGzqwogpoSMSnWZkpls"
#define API_KEY "AIzaSyCCmlb-EUSyys7BiBkO2P6bBNNw-5t-ECw"

// sensor de LUZ
#define LDR_PIN 36

// sensor de TEMPERATURA E UMIDADE
#define DHTPIN 27
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// sensor de CARGA
#define DT_PIN 22
#define SCK_PIN 23
HX711 balanca;
float fator_escala = 1.0;

// Autenticação para o firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ultimoLdrData = -1;
float ultimoTemp = -1;
float ultimaUmid = -1;

void setup() {
  Serial.begin(115200);

  // Conectar ao Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando na rede");
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    Serial.print(".");
    delay(300);
  }

  // Caso falhe a conexão
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Falha na conexão com a rede Wi-Fi!");
    return;
  }

  Serial.println();
  Serial.print("Conectado com IP: ");
  Serial.println(WiFi.localIP());

  // Configuração do Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Entrou no firebase!");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Sensor de luz
  pinMode(LDR_PIN, INPUT);

  // Inicializa o DHT22
  dht.begin();
  
  // Inicializa o HX711
  balanca.begin(DT_PIN, SCK_PIN);
  balanca.set_scale();
  balanca.tare(); // Zera a balança
  // Calibração inicial
  Serial.println("Calibração, tare (coloque o peso)...");
  delay(10000); // Aguarda 10 segundos para você colocar o peso
  long leitura_com_peso = balanca.get_units();
  Serial.print("Leitura com peso conhecido: ");
  Serial.println(leitura_com_peso);
  fator_escala = 2030.0 / leitura_com_peso; // Ajuste o fator de escala com base no peso conhecido
  balanca.set_scale(fator_escala);
}

void loop() {
  delay(2000); // Aguarda 2 segundos entres as leituras

  // Leitura do sensor de luz
  int ldrData = analogRead(LDR_PIN);
  float tempData = dht.readTemperature();
  float umidData = dht.readHumidity();
  
  // Leitura da célula de carga
  float leitura = balanca.get_units();
  float peso_kg = leitura / 1000.0; // Converte para quilogramas

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    if (ldrData < 150) {
      Serial.println("Ambiente Escuro");
    } else {
      Serial.println("Ambiente Claro");
    }

    if (abs(ldrData - ultimoLdrData) >= 100) {
      ultimoLdrData = ldrData;
      if (Firebase.setInt(fbdo, "/caixa2/luz", ldrData)) {
        Serial.println("LDR atualizado no Firebase. - " + ldrData);
      } else {
        Serial.println("ERRO: não atualizou o LDR no Firebase.");
      }
    }

    if (!isnan(tempData) && !isnan(umidData)) {
      if (abs(tempData - ultimoTemp) > 0.3) {
        ultimoTemp = tempData;
        if (Firebase.setFloat(fbdo, "/caixa2/temp", tempData)) {
          Serial.print("Temperatura enviada ao Firebase. - ");
          Serial.println(tempData);
          
        } else {
          Serial.println("ERRO ao enviar temperatura ao Firebase.");
        }
      }

      if (abs(umidData - ultimaUmid) > 1) {
        ultimaUmid = umidData;
        if (Firebase.setFloat(fbdo, "/caixa2/umidade", umidData)) {
          Serial.print("Umidade enviada ao Firebase. - ");
          Serial.println(umidData);
        } else {
          Serial.println("ERRO ao enviar umidade ao Firebase.");
        }
      }
    } else {
      Serial.println("Falha ao ler do DHT sensor!");
    }

    // Atualização do sensor de carga
    if (Firebase.setFloat(fbdo, "/caixa2/peso", peso_kg)) {
      Serial.print("Peso enviado para o Firebase");
      Serial.println(peso_kg);
    } else {
      Serial.println("Erro ao enviar peso: " + fbdo.errorReason());
    }
  }
}
