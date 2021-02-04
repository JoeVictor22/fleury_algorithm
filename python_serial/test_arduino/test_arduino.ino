int recebido = 0;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0){
    recebido = Serial.read();  
  }

  if(recebido == 5){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }else{
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)

  }
  
  Serial.println(recebido);
  
  delay(100);            
}
