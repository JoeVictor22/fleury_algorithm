#define button2 2
#define button3 3
#define MAX_SIZE 60
extern "C" char* sbrk(int incr);
extern char *__brkval;
int matriz[MAX_SIZE][MAX_SIZE];

void rodaAi();
int leitura();
int freeMemory();
void printaMemoria();
void fillMatrix();

void setup() {
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  Serial.begin(9600);
  Serial.println("\nSTART: ");
  Serial.println("Tam: ");
  Serial.println(sizeof(matriz));


  delay(100);
}
void loop() {

  int opcao = 0;
  opcao = leitura();

  //acessaUltima();
  //fillMatrix();
  //printaMemoria();
  //printaMatrix();
  /*
    if(opcao == 1){
    Serial.println("Opcao 1");
    fillMatrix();
    }else if(opcao == 2){
    Serial.println("Opcao 2");
    printaMemoria();
    }
  */
  delay(100);
}

void acessaUltima() {
  matriz[MAX_SIZE - 1][MAX_SIZE - 1] = 1000;
  Serial.print("Ultima :");
  Serial.println( matriz[MAX_SIZE - 1][MAX_SIZE - 1]);
}
void fillMatrix() {
  for (int i = 0; i < MAX_SIZE ; i++) {
    for (int j = 0; j < MAX_SIZE ; j++) {
      matriz[i][j] = i + j;
      Serial.print(matriz[i][j]);
      Serial.print("=");
      Serial.print(i);
      Serial.print("-");
      Serial.print(j);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  Serial.println(sizeof(matriz));

}

void printaMatrix() {
  for (int i = 0; i < MAX_SIZE ; i++) {
    for (int j = 0; j < MAX_SIZE ; j++) {
      Serial.print(matriz[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  Serial.println(sizeof(matriz));
}
void printaMemoria() {
  int mem = freeMemory();
  Serial.print("Memoria ");
  Serial.println(mem);
}
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}



int leitura() {
  if (digitalRead(button2) == HIGH) {
    return 1;
  } if (digitalRead(button3) == HIGH) {
    return 2;
  } else {
    return 0;
  }
}

void rodaAi() {
  int i = 0;
  while (i <= 20) {
    Serial.print("Vai ");
    Serial.println(i);
    i++;
  }
}
