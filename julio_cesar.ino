// Define as entradas e saídas na placa
#define LDR_PIN 2
#define LED_GREEN 27
#define LED_RED 26

// Criação de valores âncoras
int ultimoValorLDR = -1;
int diferenca = 100;

void setup() {
  Serial.begin(115200);
  
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Desliga todos os leds
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}

void loop() {
  // var para a leitura do LDR
  int valorLDR = analogRead(LDR_PIN);

  if (abs(valorLDR - ultimoValorLDR) >= diferenca){
    // atualiza o novo último valor do LDR
    ultimoValorLDR = valorLDR;

    Serial.println("Mudança");
    Serial.print("LDR: ");
    Serial.println(valorLDR);

    // Verifica se o ambiente está claro ou escuro
    if (valorLDR < 150){
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      Serial.println("ambiente escuro, luz vermelha");
    } else {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      Serial.println("ambiente claro, luz verde");
    }

    Serial.println("");
  }

  delay(500);
}

