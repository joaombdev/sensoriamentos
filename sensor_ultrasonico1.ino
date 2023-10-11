// C++ code

// sensor ultrasonico
int trigger = 7;
int echo = 6;

// led's
int led_verm = 8;
int led_lar = 9;
int led_ama = 10;
int led_verd = 11;
int led_bra = 12;

// variáveis
int distancia;
long tempo;

void setup(){
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led_verm, OUTPUT);
  pinMode(led_lar, OUTPUT);
  pinMode(led_ama, OUTPUT);
  pinMode(led_verd, OUTPUT);
  pinMode(led_bra, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  // acionando sensor
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  
  tempo = pulseIn(echo, HIGH);
  distancia = (tempo*0.0347)/2;
  
  Serial.print(distancia);
  Serial.println(" centimetros");
  digitalWrite(led_verm, HIGH);
  
  if (distancia < 50){
  	digitalWrite(led_verm, HIGH);
  	digitalWrite(led_lar, LOW);
  	digitalWrite(led_ama, LOW);
  	digitalWrite(led_verd, LOW);
  	digitalWrite(led_bra, LOW);
  } else if (distancia > 50 && distancia < 99){
  	digitalWrite(led_verm, LOW);
  	digitalWrite(led_lar, HIGH);
  	digitalWrite(led_ama, LOW);
  	digitalWrite(led_verd, LOW);
  	digitalWrite(led_bra, LOW);
  } else if (distancia > 100 && distancia <159){
  	digitalWrite(led_verm, LOW);
  	digitalWrite(led_lar, LOW);
  	digitalWrite(led_ama, HIGH);
  	digitalWrite(led_verd, LOW);
  	digitalWrite(led_bra, LOW);
  } else if (distancia > 160 && distancia < 199){
  	digitalWrite(led_verm, LOW);
  	digitalWrite(led_lar, LOW);
  	digitalWrite(led_ama, LOW);
  	digitalWrite(led_verd, HIGH);
  	digitalWrite(led_bra, LOW);
  } else if (distancia > 200){
  	digitalWrite(led_verm, LOW);
  	digitalWrite(led_lar, LOW);
  	digitalWrite(led_ama, LOW);
  	digitalWrite(led_verd, LOW);
  	digitalWrite(led_bra, HIGH);
  }
}

