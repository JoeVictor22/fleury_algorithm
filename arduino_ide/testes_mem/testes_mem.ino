#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

#define TAM_MATRIX_MAX 40


int tam_matrix = 0;
char grafo_matriz[TAM_MATRIX_MAX][TAM_MATRIX_MAX];
const int pos_visitado PROGMEM = TAM_MATRIX_MAX + 1;
const int pos_vetorVizinhos PROGMEM = (TAM_MATRIX_MAX * 2) + 1;
const int pos_arestas_vertice PROGMEM = (TAM_MATRIX_MAX * 3) + 1; 

void setup(){
  Serial.begin(9600); // Baudrate do terminal de entrada pinos 1,2
  Serial.setTimeout(20000);
}

void loop(){
  tamanho_mat();
  recebe_matrix();

  fleury();
  //dormir();
}

void dormir(){
  delay(200);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable(); // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  sleep_cpu();
  sleep_mode(); // here the device is actually put to sleep!!
  
  while(true){
    if(Serial.available()){
      int wakeup_call=Serial.parseInt(SKIP_ALL);
      if(wakeup_call==-1){
        // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
        sleep_disable(); // first thing after waking from sleep:
        // disable sleep...
       
        power_all_enable();
        break;
      }
    }
  }
}


void tamanho_mat(){

  while(true){
    Serial.flush();
    if (Serial.available()){
      tam_matrix=Serial.parseInt(SKIP_ALL);
      if(tam_matrix>1  && tam_matrix<=TAM_MATRIX_MAX){
        Serial.println(tam_matrix);
        break;  
      }
    }
  }
}

void recebe_matrix(){
  for (int i=0;i<tam_matrix;i++){
    for (int j=0;j<tam_matrix;j++){
      while(true){
        Serial.flush();
        if(Serial.available()){
          grafo_matriz[i][j]=Serial.parseInt(SKIP_ALL);
          Serial.print(grafo_matriz[i][j]-0);
          Serial.print(" ");
          break;
        }
      }
    }
    Serial.println();
  }
}

// Busca o vertice mais indicado para dar inicio no caminho
// Busca um vertice com grau ímpar, caso não exista nenhum, retorna um vertice qualquer do grafo
int buscarInicial(){
  int grau = 0;
  for(int i=0; i < tam_matrix; i++){
    grau = 0;
    for(int j = 0; j < tam_matrix; j++){
      if (grafo_matriz[i][j] == 1){
        grau++;
      }
    }
    if((grau % 2 ) == 1 ){
      return i;
    }
  } 
  return 0;
}

// Busca um caminho ou ciclo euleriano no grafo assim como avalia algumas caracteristicas do grafo
void fleury(){
  int start = buscarInicial();;
  // Contagem de vertices visitados
  int vertices_visitados = 0;
  int v = start;
  
   /* Calcula o tamanho maximo do caminho que pode ser percorrido
  int max_caminho_size = ((size * (size-1))/2);
  if(max_caminho_size > MAX_CAMINHO){
    max_caminho_size = MAX_CAMINHO;
  }

  // Caminho feito no grafo a ser impresso
  int caminho[max_caminho_size];
  */
  Serial.print(" ");
  Serial.print(start);  

  vertices_visitados++;
  
  while(true){

    // Quantidade de arestas naquele vertice
    int n_arestas_vertice = 0;
    n_arestas_vertice = countArestas(start);

    // Itera sobre as arestas
    int i;
    for (i = 0; i < n_arestas_vertice; i++){
    // V recebe a Aresta 
    v = lerIntEEPROM(pgm_read_word_near(pos_arestas_vertice) + i);
    
      // Verfica se a posição no Grafo é valida e se a Aresta não é de corte
      if (grafo_matriz[start][v] == 1 && ehArestaCorte(start, v, n_arestas_vertice)){
        // Caso a Aresta seja validada ela é removida do Grafo
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0;
    
        // O ponto final da Aresta é se torna o ponto inicial para as proximas verificações
        start = v;  

        // Guarda o Vertice para printar caminho
        Serial.print(" ");
        Serial.print(start);
        vertices_visitados++;
        break;
      }
    }
  
    // Caso o Vertice não seja validado anteriormente é escolha a ultima Aresta
    if(i >= n_arestas_vertice){
        // Remove a Aresta do Grafo
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0; 
    
        // O ponto final da Aresta é se torna o ponto inicial para as proximas verificações
        start = v;
    
        // Guarda o Vertice para printar caminho
        Serial.print(" ");
        Serial.print(start);
        vertices_visitados++;
    }

    // Caso de saida uma Vertice que não possui Arestas
    if(n_arestas_vertice == 0){
      // Remove pois visita a si mesmo garantindo ser o ultimo ponto
      vertices_visitados--; 
      break;
    }
  }

  // Printa o Grafo após percorer todos os caminhos validos
  //printGrafo(size);

  // Um Grafo contem um caminho Euleriano se somente se ele percore todas as arestas contidas nele.
  if (qtdArestas() > 0){
    Serial.println();
    Serial.println("Grafo nao contem caminho Fleuriano");
  }
  printGrafo();
  Serial.println(" ");
}

// Retorna a quantidade de arestas de um vertice e salva essas vertices em um vetor 
int countArestas(int row){
    int count = 0;
    for(int i = 0; i < tam_matrix; i++){
        if(grafo_matriz[row][i] == 1){
            escreverIntEEPROM(pgm_read_word_near(pos_arestas_vertice) + count, i);
            count++;
        }
    }
    return count;
}

// Valida a Aresta do grafo a ser utilizada
bool ehArestaCorte(int start, int end, int n_arestas_vertice){
  // Contabiliza a quantidade de ligações que o Vertice possui
  int count = 0;
  for (int i = 0; i < n_arestas_vertice; i++){ 
    if(grafo_matriz[start][i] == 1){
      count++;
    }
  }
  
  // Caso o Vertice só possua uma Aresta é realizado uma DFS a fim de reconhecer se ele é uma "Aresta de corte"
  if (count == 1){
    // Salva a quantidade de Vertices acessiveis com a Aresta presente no grafo
    int dfs_c_aresta = dfs(end);
    
    grafo_matriz[end][start] = 0;
    grafo_matriz[start][end] = 0;

    // Salva a quantidade de Vertices acessiveis sem a Aresta presente no grafo
    int dfs_s_aresta = dfs(end);

    grafo_matriz[end][start] = 1;
    grafo_matriz[start][end] = 1;

    // Ao comparar o resultado retornado pelas DFS's é definido se a aresta será usada.
    return (dfs_c_aresta > dfs_s_aresta) ? false : true;
  
  /* Caso não seja possivel determinar via uma dfs, essa aresta sera considerada de corte se e somente se a quantidade de
   vertices conectados seja igual a 0
  */
  }else if(count > 1){
    return false;
  }else{
    return true;
  }
}


void iniciarVetorIntEEPROM(int pos, int valor){
  for(int i = 0; i < tam_matrix; i++){
    escreverIntEEPROM(pos + i, valor);
  }
}
void iniciarVetorBoolEEPROM(int pos, bool valor){
  for(int i = 0; i < tam_matrix; i++){
    escreverBoolEEPROM(pos + i, valor);
  }
}

void escreverBoolEEPROM(int pos, bool valor){
  byte* p = (byte*)(void*)&valor;
  for (int i = 0; i < sizeof(valor); i++)
  {
    EEPROM.write(pos++, *p++);
  }
}
void escreverIntEEPROM(int pos, int valor){
  byte* p = (byte*)(void*)&valor;
  for (int i = 0; i < sizeof(valor); i++)
  {
    EEPROM.write(pos++, *p++);
  }
}

bool lerBoolEEPROM(int pos){
  bool valor = false;
  byte* p = (byte*)(void*)&valor;
  for (int i = 0; i < sizeof(valor); i++)
  {
  *p++ = EEPROM.read(pos++);
  }
  return valor;
}
int lerIntEEPROM(int pos){
  int valor = 0;
  byte* p = (byte*)(void*)&valor;
  for (int i = 0; i < sizeof(valor); i++)
  {
  *p++ = EEPROM.read(pos++);
  }
  return valor;
}

// Copia somente uma linha do Grafo para um vetor
void copiarGrafoLinha(int row){   
    for(int j = 0; j < tam_matrix; j++){
        escreverIntEEPROM(pgm_read_word_near(pos_vetorVizinhos) + j, grafo_matriz[row][j]);
    }
}

int dfs(int inicial){
    // Pilha de proximos a entrar na DFS
    int pilha[tam_matrix];
    iniciarVetor(pilha);

  // Vetor boolenao armazenando vertices que são possiveis ser acessados
    iniciarVetorBoolEEPROM(pgm_read_word_near(pos_visitado), false);
    for(int i = 0; i < tam_matrix; i++){
        escreverBoolEEPROM(pgm_read_word_near(pos_visitado) + i, false);
        //visitado[i] = false;
    }

  // A primeira vertice na DFS é o inicial passado como argumento
    empurrar(pilha, inicial);
    escreverBoolEEPROM(pgm_read_word_near(pos_visitado) + inicial, true);
    //visitado[inicial] = true;
  
  // Realiza verificações até a pilha estar vazia
    while(!ehVazio(pilha)){ 
        
    // Vertice é a vertice onde ira ser feito a DFS
        int vertice;
        vertice = sacar(pilha);
        
        // Recebe um vetor que é a linha da Matriz correspondente ao Vertice a ser feita a DFS
        //int vetorVizinhos[size];
        copiarGrafoLinha(vertice);
        
    // "i" itera sobre o vetor e marca os vizinhos ao Vertice utilizado
        for(int i=0; i < tam_matrix; i++){
      // Caso o vertice seja vizinho, (==1), e ainda não tenha sido marcado como visitado
      // O vizinho será colocado na pilha para verificar os seus proprios vizinhos e marcado como visitado
            //if(vetorVizinhos[i] == 1 && !visitado[i]){
            if(lerIntEEPROM(pgm_read_word_near(pos_vetorVizinhos) + i) == 1 && !lerBoolEEPROM(pgm_read_word_near(pos_visitado) + i)){
                empurrar(pilha, i);

                //visitado[i] = true;
                escreverBoolEEPROM(pgm_read_word_near(pos_visitado) + i, true);
            }
        }
    }

  // Realiza uma contagem de quantos vertices é possivel atingir a partir do inicial
    int count = 0;
    for(int i = 0; i < tam_matrix; i++){
        //if(visitado[i] == true){
        if(lerBoolEEPROM(pgm_read_word_near(pos_visitado) + i) == true){
            count++;
        }
    }

  // Retornar a contagem para comparações
    return count;
}


/*--------------------------------------------PILHA-------------------------------------------------------*/
void iniciarVetor(int *vector){
    for(int i=0; i < tam_matrix;i++){
        vector[i] = -1;
    }
}

// Verifica se a pilha esta preenchida com somente valores -1
// Caso verdade a pilha se encontra vazia, retorna TRUE.
bool ehVazio(int *pilha){
    for(int i=0; i < tam_matrix; i++){
        if(pilha[i] != -1){
            return false;
        }    
    }
    return true;
}

// Insere um valor no topo da pilha
void empurrar(int *pilha, int value){
    int i;
    for(i = 0; i < tam_matrix; i++){
        if(pilha[i] == -1){
            break;
        }
    }
    pilha[i] = value;
}

// Saca o valor que se encontra no topo da pilha
int sacar(int *pilha){
    int i;
    for(i=0; i < tam_matrix; i++){
        if(pilha[i] == -1){
            break;
        }    
    }
    int aux = pilha[i-1];
    pilha[i-1] = -1;
    return aux;
}

/*--------------------------------------------------------------------------------------------------------*/

// Printa o Grafo em sua integridade
void printGrafo(){
    Serial.println();
    for( int i =0 ; i < tam_matrix; i++){
        for( int j= 0; j < tam_matrix; j++){
            Serial.print(grafo_matriz[i][j]-0);
            Serial.print(" ");
        }
        Serial.println();
    }
}

// Calcula a quantidade de arestas presentes no grafo
int qtdArestas(){
  int arestas=0;
  for (int i=0;i<tam_matrix;i++){
    for (int j=i;j<tam_matrix;j++){
      arestas+=grafo_matriz[i][j];
    }
  }
  return arestas;
}