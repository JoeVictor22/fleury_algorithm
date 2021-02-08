
/* Comentarios neste formato descrevem variaveis ou funções resumidamente */

/*
  Comentarios neste formato descrevem blocos maiores, seja uma função ou um fluxo

  No caso de funções, o retorno pode ser descrito da seguinte forma:
  Retorno: o retorno da função de forma resumida e seu tipo
*/

/*-----------------------------OBSERVAÇÃO----------------------------------
*
*  Comentarios neste formato serão usados para observações ou notas   
*
--------------------------------------------------------------------------*/

/*
  TODO: Explicar a comunicação serial
*/
/*
  TODO: Explicar a estrutura de dados
*/
/*
  TODO: Explicar a lógica do algoritmo
*/
/*
  TODO: Trocar EEPROM.write por EEPROM.update
*/

/* Blibiotecas usados para reduzir o consumo de energia */
#include <avr/power.h>
#include <avr/sleep.h>
/* Bibliotecas usadas para reduzir o uso de memória */
#include <avr/pgmspace.h>
#include <EEPROM.h>
/* Quantidade de vertices máxima aceitada pela aplicação, isso resulta em TAM_MATRIX_MAX^2 bytes alocados no escopo global */
#define TAM_MATRIX_MAX 40
/* Quantidade minima de vertices permitida */
#define TAM_MATRIX_MIN 1

/* Variável que indica a quantidade de linhas X colunas que serão computadas nessa iteração, esse valor será recebido via serial 
no inicio de cada iteração e será usado durante a computação para definir a quantidade de vertices usados */
int tam_matrix = 0;
/* Declaração da estrutura de dados utilizada pela aplicação */
char grafo_matriz[TAM_MATRIX_MAX][TAM_MATRIX_MAX];
/* Constantes utilizadas para o armazenamento de vetores na EEPROM, cada constante indica o endereço de um vetor na EEPROM */
/* As constantes foram armazenadas em Flash como uma tentativa de reduzir o consumo de SRAM */
const int pos_visitado PROGMEM = TAM_MATRIX_MAX + 1;
const int pos_vetorVizinhos PROGMEM = (TAM_MATRIX_MAX * 2) + 1;
const int pos_arestas_vertice PROGMEM = (TAM_MATRIX_MAX * 3) + 1; 



/*
  Durante o setup, definimos o baudrate das portas seriais no pino 1 e 2, e definimos um timeout de 20 segundos para a comunicação serial
*/
void setup(){
  Serial.begin(9600);
  Serial.setTimeout(20000);
}


/*
  Durante o loop, a placa aguarda a quantidade de vertices que serão computados, em seguida, recebe a matriz de adjacencia, computa os 
  dados e por fim, coloca a placa em um modo de consumo de energia
*/
void loop(){
  /* Recebe a quantidade de vertices */
  tamanho_mat(); 
  /* Recebe a matriz de adjacencia */
  recebe_matrix();

  /* Computa a matriz */
  fleury();
  /* Entre em um modo de consumo */
  dormir();
}


/*
  TODO: Escrever o fluxo do sleep e descrever resumidamente cada funcao
*/
void dormir(){
  delay(200);
  
  set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here
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



/*
  Recebe a quantidade de vertices que irá definir o tamanho da matriz utilizado durante a computação
*/
void tamanho_mat(){
  /* Trava o programa em um laço até que seja enviado um inteiro */
  while(true){
    /* Limpa a serial para garantir que seja recebido blocos pequenos, de inteiro por inteiro */
    Serial.flush();
    /* Caso exista bytes disponiveis para leitura */
    if (Serial.available()){
      /* Busca o proximo inteiro valido na serial e ignora qualquer outro caractére */
      tam_matrix=Serial.parseInt(SKIP_ALL);
      /* Verifica se o inteiro inserido esta dentre os valores permitidos */
      if(tam_matrix > TAM_MATRIX_MIN && tam_matrix <= TAM_MATRIX_MAX){
        /* Envia para a serial o valor aceito e quebra o laço */
        Serial.println(tam_matrix);
        break;  
      }
    }
  }
}

/*
  Recebe a matriz de adjacencia e armazena os elementos na matriz definida globalmente
*/
void recebe_matrix(){
  /* Para cada elemento do matriz, a placa é presa em um laço aguardando o envio daquele elemento pela serial */
  for (int i = 0; i < tam_matrix; i++){
    for (int j = 0; j < tam_matrix; j++){
      while(true){
        /* Limpa a serial para garantir que seja recebido blocos pequenos, de inteiro por inteiro */
        Serial.flush();
        /* Caso exista bytes disponiveis para leitura */
        if(Serial.available()){
          /* Busca o proximo inteiro valido na serial ignorando qualquer outro caractére e armazena o valor na sua posição 
          correspondente da matriz */
          grafo_matriz[i][j] = Serial.parseInt(SKIP_ALL);
          /* Envia para a serial o valor armazenado e quebra o laço */
          Serial.print(grafo_matriz[i][j]-0);
          Serial.print(" ");
          break;
        }
      }
    }
    /* Quebra de linha para melhorar a visualização */
    Serial.println();
  }
}

/*
  Função utilizada para obter um vertice em que sejá possivel dar inicio a uma trilha euleriana
  Retorno: um inteiro indicando a indexação de um vertice
*/
int buscarInicial(){
  /* Variavel que armazena o grau de cada vertice visitado durante o calculo */
  int grau = 0;
  /* Visita cada vertice 'i' da matriz ate encontrar um favoravel */
  for(int i = 0; i < tam_matrix; i++){
    /* Reseta a quantidade de graus para dar inicio a contagem */
    grau = 0;
    /* Itera sobre todos os vertices conectados ao vertice 'i' e incrementa a contagem de graus caso exista aresta 
    conectada ao vertice 'j' */
    for(int j = 0; j < tam_matrix; j++){
      if (grafo_matriz[i][j] == 1){
        grau++;
      }
    }
    /* Após verificar todos os vertices conectados, verifica se o grau é impar e retorna esse vertice 
    para ser utilizado*/
    if((grau % 2 ) == 1 ){
      return i;
    }
  } 
  /* Caso não exista vertices de grau impar, retorna 0 para o algoritmo utilizar o primeiro vertice como inicial*/
  return 0;
}

/*
  Função na qual é realizada a computação e obtenção da trilha
*/
void fleury(){
  /* Obtem o vertice inicial da trilha */
  int start = buscarInicial();
  /* Contador de vertices, não repetidos, visitados pela trilha */
  int vertices_visitados = 0;
  
  /* 
    'v' e 'start' serão utilizados no decorrer dessa função, as duas variáveis simbolizam vertices que se 
    conectam via uma aresta e durante a computação as duas variáveis irão assumir diferentes valores obtidos 
    durante a execução do algoritmo.
  */

  /* ------------------------------Divergência com a versão do liunux---------------------------------------
  *  Diferente da versão implementada no linux, optamos por enviar pela Serial cada vertice do caminho durante 
  *  a execução ao inves de armazena-las em um vetor para ser consultado posteriormente, isso fez com que a 
  *  ultima vertice enviada pelo serial seja repetida, já que durante a execução do fluxo a seguir, a ultima 
  *  vertice é considerada valida para a trilha e é computada e enviada para a Serial. 
  ---------------------------------------------------------------------------------------------------------*/

  /* Atribui o valor da vertice de inicio a vertice 'v' que será verificada a seguir */
  int v = start;
  
  /* Envia para a serial a primeira vertice do caminho */
  Serial.print(" ");
  Serial.print(start);  

  /* Incrementa o contador de vertices*/
  vertices_visitados++;
  
  /* O algoritmo é preso em um laço, sua condição de saída é garantida para caso uma vertice possua 0 arestas, 
  indicando que o caminho chegou ao fim */
  while(true){

    /* Número de arestas do vertice que está sendo verificado */
    int n_arestas_vertice = 0;
    /* countArestas é responval por calcular o número de arestas de 'start' e armazena-las na EEPROM da placa */
    n_arestas_vertice = countArestas(start);

    /* A variável 'i' é utilizada para iterar sobre as arestas do vertice e será usada para verificar se foi 
    possivel encontrar uma vertice valida para o caminho */
    int i;
    for (i = 0; i < n_arestas_vertice; i++){
    /* 'v' recebe da EEPROM o valor da aresta */
    v = lerIntEEPROM(pgm_read_word_near(pos_arestas_vertice) + i);
    
      
      /* Verifica se no par [start][v] existe aresta e se essa aresta, caso exista, é uma aresta de corte e 
      pode ser usada no caminho */
      if (grafo_matriz[start][v] == 1 && ehArestaCorte(start, v, n_arestas_vertice)){
        /* A aresta é removida da matriz de adjancecia para nao ser utilizada nas proximas iteracoes */
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0;
    
        /* O vertice visitada, agora passa a ser o próximo vertice utilizada como referencia de inicio para 
        a verificação do caminho */
        start = v;  

        /* Envia o caminho obtido pela Serial e incrementa o contador de vertices vistados no grafo */
        Serial.print(" ");
        Serial.print(start);
        vertices_visitados++;

        /* Quebra o laço interno 'for' para ser iterado a próxima etapa do caminho */
        break;
      }
    }
  
    /* Caso não seja obtido um vertice valido, o algoritmo escolhe a ultima aresta verificada como caminho 
    escolhido */
    if(i >= n_arestas_vertice){
        /* A aresta é removida da matriz de adjancecia para nao ser utilizada nas proximas iteracoes */
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0; 
    
        /* O vertice visitada, agora passa a ser o próximo vertice utilizada como referencia de inicio para 
        a verificação do caminho */
        start = v;
    
        /* Envia o caminho obtido pela Serial e incrementa o contador de vertices vistados no grafo */
        Serial.print(" ");
        Serial.print(start);
        vertices_visitados++;
    }

    /* Caso de saida do laço 'while', se o vertice visitado não possui nenhuma aresta conectada, 
    é decrementado o contador e o laço é quebrado */
    if(n_arestas_vertice == 0){
      /* O vertice é decrementado, pois esse caso de quebra indica que o vertice visitou a si mesmo */
      vertices_visitados--; 
      break;
    }
  }

  /* Segundo o algoritmo, para verificarmos se a trilha obtida é valida, verificamos se todos as 
  arestas do grafo foram visitadas e destruidas */
  if (qtdArestas() > 0){
    /* Retorna a Serial uma mensagem informando que não possui caminho */
    Serial.println();
    Serial.println("Grafo nao contem caminho Fleuriano");
  }
  /* Retorna o grafo apos a execução do algoritmo, com ou sem arestas destruidas */
  printGrafo();
  Serial.println(" ");
}

/*
  Obtem o numero de arestas conectadas a dado vertice e armazena esses vertices na EEPROM para serem
  lidos pelo algoritmo
  Entrada: inteiro indicando o vertice que será usado como referência
  Retorno: inteiro indicando a quantidade de arestas conectadas aquele vertice
*/
int countArestas(int row){
    /* Contador de vertices adjacentes */
    int count = 0;
    /* Itera sobre todos os vertices da matriz e verifica se cada um deles possui uma aresta conectada com
    o vertice de referencia */
    for(int i = 0; i < tam_matrix; i++){
        if(grafo_matriz[row][i] == 1){
            /* Caso exista aresta, é salvo o valor na EEPROM e o contador é incrementado */
            escreverIntEEPROM(pgm_read_word_near(pos_arestas_vertice) + count, i);
            count++;
        }
    }
    return count;
}

/*
  Realiza diversas verificações para verificar se a aresta que conecta os dois vertices pode ser utilizada
  no caminho.

  Entrada: 
  'start' = inteiro indicando vertice de referencia,
  'end' = inteiro indicando vertice a ser verificado,
  'n_arestas_vertice' = inteiro indicando a quantidade de vertices que estão conectados ao vertice de referencia 
 
  Saida: boleano indicando se o vertice 'end' esta conectado ao 'start' pode ser utilizado no caminho
*/
/*
* Nesta função, o boleano indicara que a aresta pode ser utilizada no caminho e não se ela é uma aresta de corte
*/
bool ehArestaCorte(int start, int end, int n_arestas_vertice){
  /* Contador indicando a quantidade de arestas do vertice 'start' */
  int count = 0;
  /* Contabiliza as arestas de 'start' */
  for (int i = 0; i < n_arestas_vertice; i++){ 
    if(grafo_matriz[start][i] == 1){
      count++;
    }
  }
  
  /* Caso o vertice 'start' possue apenas uma aresta conectada ao vertice 'end' e nenhuma outra, é utilizada 
  uma DFS para determinar se aquela aresta pode ser utilizada no caminho */
  if (count == 1){
    /* Salva a quantidade de vertices que podem ser alcançados apartir do vertice 'end' com a aresta de 'start' para 'end'*/
    int dfs_c_aresta = dfs(end);
    
    /* Remove a aresta da matriz */
    grafo_matriz[end][start] = 0;
    grafo_matriz[start][end] = 0;

    /* Salva a quantidade de vertices que podem ser alcançados apartir do vertice 'end' caso a aresta seja removida */
    int dfs_s_aresta = dfs(end);

    /* Insere a aresta de volta a matriz */
    grafo_matriz[end][start] = 1;
    grafo_matriz[start][end] = 1;

    /* Caso a remoção da aresta reduza o nummero de vertices acessiveis, é retornado 'false' indicado que não deve ser 
    utilizado essa aresta, caso contrario é retornado 'true' */
    return (dfs_c_aresta > dfs_s_aresta) ? false : true;
  
  /* Caso não seja possivel determinar via uma DFS, é retornado 'false' para se exister mais arestas conectadas, caso 
  contrario, será retornado 'true' o que irá levar o algoritmo a entender que esse é o ultimo caminho, pois o numero de 
  areastas conectadas ao vertice é 0 */
  }else if(count > 1){
    return false;
  }else{
    return true;
  }
}


/* 
  Inicia um vetor de inteiros na memoria EEPROM.
  Entrada:
  'pos' = inteiro indicando o endereco de memoria em que o vetor começa
  'valor' = inteiro que será armazenado em todo o vetor
*/
void iniciarVetorIntEEPROM(int pos, int valor){
  for(int i = 0; i < tam_matrix; i++){
    /* Escreve na posicao de memoria */
    escreverIntEEPROM(pos + i, valor);
  }
}
/* 
  Inicia um vetor de boleanos na memoria EEPROM.
  Entrada:
  'pos' = inteiro indicando o endereco de memoria em que o vetor começa
  'valor' = boleano que será armazenado em todo o vetor
*/
void iniciarVetorBoolEEPROM(int pos, bool valor){
  for(int i = 0; i < tam_matrix; i++){
    /* Escreve na posicao de memoria */
    escreverBoolEEPROM(pos + i, valor);
  }
}

/* 
  Escreve um boleano em dada posicão de memoria EEPROM.
  Entrada:
  'pos' = inteiro indicando o endereco de memoria em que sera armazenado o valor
  'valor' = boleano que será armazenado em todo o vetor
*/
void escreverBoolEEPROM(int pos, bool valor){
  /* byte com o valor a ser escrito */
  byte* p = (byte*)(void*)&valor;
  for (int i = 0; i < sizeof(valor); i++){
    /* chamada EEPROM para escrita */
    EEPROM.write(pos++, *p++);
  }
}
/* 
  Escreve um inteiro em dada posicão de memoria EEPROM.
  Entrada:
  'pos' = inteiro indicando o endereco de memoria em que sera armazenado o valor
  'valor' = inteiro que será armazenado em todo o vetor
*/
void escreverIntEEPROM(int pos, int valor){
  /* byte com o valor a ser escrito */
  byte* p = (byte*)(void*)&valor;
  for (int i = 0; i < sizeof(valor); i++)
  {
    /* chamada EEPROM para escrita */
    EEPROM.write(pos++, *p++);
  }
}
/* 
  Realiza uma leitura em dada posicão de memoria EEPROM e retorna o valor armazenado nela.
  Entrada:
  'pos' = inteiro indicando o endereco de memoria em que sera realizado a leitura
  Saida: boleano armezanado naquele endereço
*/
bool lerBoolEEPROM(int pos){
  /* Variável usada para obter o tamanho do dado que será lido */
  bool valor = false;
  /* Define um byte para armazenar o valor lido na memoria */
  byte* p = (byte*)(void*)&valor;

  /* Lê o tamanho em bytes da posição de memoria e armazena no byte 'p' */
  for (int i = 0; i < sizeof(valor); i++){
  *p++ = EEPROM.read(pos++);
  }
  /* Retorna o valor lido */
  return valor;
}
/* 
  Realiza uma leitura em dada posicão de memoria EEPROM e retorna o valor armazenado nela.
  Entrada:
  'pos' = inteiro indicando o endereco de memoria em que sera realizado a leitura
  Saida: inteiro armezanado naquele endereço
*/
int lerIntEEPROM(int pos){
  /* Variável usada para obter o tamanho do dado que será lido */
  int valor = 0;
  /* Define um byte para armazenar o valor lido na memoria */
  byte* p = (byte*)(void*)&valor;

  /* Lê o tamanho em bytes da posição de memoria e armazena no byte 'p' */
  for (int i = 0; i < sizeof(valor); i++){
  *p++ = EEPROM.read(pos++);
  }
  /* Retorna o valor lido */
  return valor;
}

/*
  Realiza a copia de uma linha da matriz na memoria EEPROM
  Entrada: inteiro indicando a linha a ser copiada
*/
void copiarGrafoLinha(int row){   
  /* Itera sobre toda a linha e escreve um inteiro na EEPROM no endereco de memoria do vetor de vertices vizinhas */
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