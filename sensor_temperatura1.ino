// C++ code
//

// var para temperaturas
int sensorPin = A0;
float valorSensor;
float tensaoSaida;
float temperaturaC;
  
// var para led's
int ledBlue = 3;
int ledGreen = 4;
int ledRed = 5;

  
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sensorPin, INPUT);
  
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  // Habilitar porta serial em uma taxa de 9600
  //Serial.begin(9600);
}

void loop()
{
  // apenas para acender o led_builtin
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
  
  // Ler o sensor e salvar
  valorSensor = analogRead(sensorPin);
  
  // Encontrando a porcentagem da leitura de input
  temperaturaC = valorSensor / 1024;
  
  // Multiplique por 5V para obter a tensão
  temperaturaC = temperaturaC * 5;
  
  // Subtraia o deslocamento 
  temperaturaC = temperaturaC - 0.5;
  
  // Calc para valor celsius
  temperaturaC = temperaturaC * 100;
  
  // laço de repetições para os leds
  if (temperaturaC < 10){
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
  } else if (temperaturaC > 10 && temperaturaC < 65){
  	digitalWrite(ledBlue, LOW);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
  } else if (temperaturaC > 65){
  	digitalWrite(ledBlue, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);
  }
  
  // Printando temp (em graus Cº)
  Serial.print("\n Temperatura(celsius): ");
  Serial.print(temperaturaC);
}	