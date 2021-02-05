

int recebido;
int vet[10];
int i = 0;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  //Serial.println("Terminal de ENTRADA");
}

void loop() {
  
  if (1Serial.available() && i < 10) {
    recebido = Serial.read();
    Serial.println(recebido);
    vet[i] = recebido;
    i++;
  }

  if (recebido == 1) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  }

  if(i == 10){
    while( i > 0){
      Serial.println(vet[i-1]);            
      i--;
    }
  }
  
  delay(100);
}
