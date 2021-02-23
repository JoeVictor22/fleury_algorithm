
/* Comentarios neste formato descrevem variaveis ou funções resumidamente */

/*
  Comentarios neste formato descrevem blocos maiores, seja uma função ou um fluxo

  No caso de funções, o retorno pode ser descrito da seguinte forma:
  Entrada: 
  'var1' = definicao da variavel
  'var2' = definicao da variavel
  Saida: o retorno da função de forma resumida e seu tipo
*/

/*-----------------------------OBSERVAÇÃO----------------------------------
*
*  Comentarios neste formato serão usados para observações ou notas   
*
--------------------------------------------------------------------------*/



/*-----------------------------LICENSE--------------------------------------
* Implementação Iterativa do algoritimo de Fleury com busca em profundidade
* para a plataforma Arduino UNO
* Copyright © 2021 by Joel Victor Castro Galvão, Raynan Serafim de Souza
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all 
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------*/

/*----------------------------CONSIDERAÇÕES------------------------------------
* O programa foi desenvolvido no contexto de um trabalho academico, com o 
* intuito de implementar o algoritmo de fleury em uma plataforma com limitações
* de hardware.
* O arduino UNO atmega328p foi a plataforma alvo de nosso programa, durante o
* desenvolvimento da aplicação, foram utilizados o proteus, o SimulIDE e uma
* placa arduino fisíca para a validação e testes do programa.
* Funções e constantes foram definidas em camelCase e variáveis em snake_case.
* Para a implementação do algoritmo, utilizamos as seguintes fontes como 
* referéncia.
*   - https://www.inf.ufsc.br/grafos/temas/euleriano/euleriano.htm
*   - http://www.gpec.ucdb.br/pistori/disciplinas/discreta/aulas/geulham.htm
*   - https://www.youtube.com/watch?v=mRvt5yJTw7E
*   - http://tmrfindia.org/sutra/v2i21.pdf
*   - https://www.geeksforgeeks.org/eulerian-path-and-circuit/
*   - https://www.tutorialspoint.com/fleury-s-algorithm-for-printing-eulerian-path-or-circuit-in-cplusplus
*   - https://www.tutorialspoint.com/Fleury-s-Algorithm
*   - https://www.geeksforgeeks.org/implementation-of-bfs-using-adjacency-matrix/
* Para o port da aplicação no arduino, consultamos as seguintes fontes e 
* códigos exemplo durante o desenvolvimento
*   - http://www.nongnu.org/avr-libc/user-manual/power_8h.html
*   - http://www.nongnu.org/avr-libc/user-manual/sleep_8h.html
*   - https://www.electroniclinic.com/arduino-sleep-modes-automatic-and-manual-to-save-power-arduino-deep-sleep/
*   - https://www.arduino.cc/reference/en/
*   - https://create.arduino.cc/projecthub/john-bradnam/reducing-your-memory-usage-26ca05
--------------------------------------------------------------------------*/

/*----------------------ESTRUTURA DE DADOS----------------------------------
* O programa consiste em uma matriz de adjacencia de tamanho TAM_MATRIX_MAX^2,
* que representa o grafo e suas arestas.
* O programa utilza cerca de 87% dos 2KB de SRAM, sobrando cerca de 256 bytes
* para variáveis locais. É esperado que a maior pilha de memoria que variaveis
* locais irão precisar será de cerca de 140 bytes, devido a um vetor de inteiros
* detamanho TAM_MATRIX_MAX e alguns contadores e iteradores.
* Foi implementado uma estrutura de pilha que trabalha especificamente para a
* resolução do algoritmo.
* Dentre as variáveis locais, optamos por definir alguns vetores dentro da 
* memória EEPROM e constantes na Flash.     
--------------------------------------------------------------------------*/

/*-----------------------------LÓGICA--------------------------------------
* O programa consiste no recebimento de um inteiro que representa o número de
* vertices e será usado para definir a quantidade de elementos armazenados
* na matriz de adjacencia. Após receber esse inteiro, o programa aguarda o
* recebimento dos N x N elementos da matriz e então inicia a computação.
* Já o algoritmo, consiste em visitar todos os vertices do grafo, verificar 
* quais arestas conectadas não são arestas de corte, e transversar o grafo
* utilizando apenas essas arestas que não irão gerar perda de vertices.
* Durante a execução, os vertices visitados pelo algoritmo serão enviados
* via serial para uma interface conectada.
* Caso não seja possivel obter uma trilha euleriana, o algoritmo ira enviar
* via serial uma mensagem de erro.
* Após a execução do ciclo descrito, o programa coloca a placa em um modo
* IDLE, para reduzir o consumo de energia. Para tirar a placa desse estado
* e recomeçar a execução, basta enviar '-1' como sinal de WAKE.  
--------------------------------------------------------------------------*/

/* Blibiotecas usadas para tentar reduzir o consumo de energia */
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
char grafo_matriz[TAM_MATRIX_MAX*TAM_MATRIX_MAX];
/* Constantes utilizadas para o armazenamento de vetores na EEPROM, cada constante indica o endereço de um vetor na EEPROM */
/* As constantes foram armazenadas em Flash como uma tentativa de reduzir o consumo de SRAM */
const int POS_VISITADO PROGMEM = TAM_MATRIX_MAX + 1;
const int POS_VETOR_VIZINHOS PROGMEM = (TAM_MATRIX_MAX * 2) + 1;
const int POS_ARESTAS_VERTICE PROGMEM = (TAM_MATRIX_MAX * 3) + 1; 
/* Mensagem para caso não seja possivel gerar uma trilha euleriana, armazenada em Flash */
const char NOT_EULER[] PROGMEM = {"Grafo não possui caminho Fleuriano!\n"};



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
  tamanhoMat(); 
  /* Recebe a matriz de adjacencia */
  recebeMatriz();
  /* Computa a matriz */
  fleury();
  /* Entre em um modo de consumo */
  dormir();
}


/*
  Coloca a placa em um modo de sleep em IDLE numa tentativa de reduzir o consumo de energia
*/
void dormir(){
  delay(200);
  /* Seta o tipo de sleep a ser realizado */
  set_sleep_mode(SLEEP_MODE_IDLE); 
  /* Permite o sleep */
  sleep_enable(); 
  /* Desativa o ADC(Analog to Digital converter) presente na atmega */
  power_adc_disable(); 
  /* Desativa o SPI(Serial Peripheral Interface) presente na atmega */
  power_spi_disable(); 
  /* Desativa o TIMER0(8-bit timer) presente na atmega */
  power_timer0_disable(); 
  /* Desativa o TIMER1(16-bit timer) presente na atmega */
  power_timer1_disable(); 
  /* Desativa o TIMER2(8-bit timer) presente na atmega */
  power_timer2_disable(); 
  /* Desativa o TWI(two-wire interface) presente na atmega */
  power_twi_disable(); 
  /* Ativa o sleep */
  sleep_mode(); 


  while(true){
    if(Serial.available()){
      /* Acorda quando recebe -1 na serial */
      if(Serial.parseInt(SKIP_ALL)==-1){
        /* Desativa o sleep, Reativa todas as partes desativadas antes do sleep e quebra o laço */
        sleep_disable(); 
        power_all_enable(); 
        break; 
      }
    }
  }
}


int calculaMatrizPos(int i, int j){
  if(i < j){
    return (i*(tam_matrix-1) - (i-1)*((i-1) + 1)/2 + j - i - 1);
  }else if (i > j){
      return (j*(tam_matrix-1) - (j-1)*((j-1) + 1)/2 + i - j - 1);
  }else{
    return -1;
  }
}
void escreveMatriz(int valor, int i, int j){
  if(i != j){
    grafo_matriz[calculaMatrizPos(i,j)] = valor;
  }
}
int lerMatriz(int i, int j){
  if (i != j){
    return grafo_matriz[calculaMatrizPos(i,j)];
  }else{
    return 0;
  }

}

/*
  Recebe a quantidade de vertices que irá definir o tamanho da matriz utilizado durante a computação
*/
void tamanhoMat(){
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
void recebeMatriz(){
  /* Para cada elemento do matriz, a placa é presa em um laço aguardando o envio daquele elemento pela serial */
  for (int i = 0; i < tam_matrix; i++){
    for (int j = 0; j < tam_matrix; j++){
      while(true){
        /* Limpa a serial para garantir que seja recebido blocos pequenos, de inteiro por inteiro */
        Serial.flush();
        /* Caso exista bytes disponiveis para leitura */
        if(Serial.available()){
          /* Busca o proximo inteiro valido na serial ignorando qualquer outro caractér, valida o valor e armazena o
          valor na sua posição correspondente da matriz */
          int aux = Serial.parseInt(SKIP_ALL);
          if( aux == 1 || aux == 0){
            
            /*ALTER
            grafo_matriz[i][j] = aux;
            */
            if( j > i){
              escreveMatriz(aux, i, j);
            }
            /* Envia para a serial o valor armazenado e quebra o laço */
            Serial.print(lerMatriz(i,j) - 0);
            Serial.print(" ");
            break;
          }

        }
      }
    }
    /* Quebra de linha para melhorar a visualização */
    Serial.println();
  }
}

/*
  Função utilizada para obter um vertice em que sejá possivel dar inicio a uma trilha euleriana
  Saida: um inteiro indicando a indexação de um vertice
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
      /*ALTER
      if (grafo_matriz[i][j] == 1){
      */
      if (lerMatriz(i,j) == 1){
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
    v = lerIntEEPROM(pgm_read_word_near(POS_ARESTAS_VERTICE) + i);
    
      /* Verifica se no par [start][v] existe aresta e se essa aresta, caso exista, é uma aresta de corte e 
      pode ser usada no caminho */
      /*ALTER
      if (grafo_matriz[start][v] == 1 && ehCaminhoValido(start, v, n_arestas_vertice)){
      */
      if (lerMatriz(start,v) == 1 && ehCaminhoValido(start, v, n_arestas_vertice)){
        /* A aresta é removida da matriz de adjancecia para nao ser utilizada nas proximas iteracoes */
        /*ALTER
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0;
        */
        escreveMatriz(0,start,v);
    
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
        /*ALTER
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0;
        */
        escreveMatriz(0,start,v);
    
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

    char aux;
    /* Lê byte por byte a mensagem de saida armazenada na Flash */
    for (byte k = 0; k < strlen_P(NOT_EULER); k++) {
      aux = pgm_read_byte_near(NOT_EULER + k);
      Serial.print(aux);
    }


  }
  /* Retorna o grafo apos a execução do algoritmo, com ou sem arestas destruidas */
  printGrafo();
  Serial.println(" ");
}

/*
  Obtem o numero de arestas conectadas a dado vertice e armazena esses vertices na EEPROM para serem
  lidos pelo algoritmo
  Entrada: inteiro indicando o vertice que será usado como referência
  Saida: inteiro indicando a quantidade de arestas conectadas aquele vertice
*/
int countArestas(int row){
    /* Contador de vertices adjacentes */
    int count = 0;
    /* Itera sobre todos os vertices da matriz e verifica se cada um deles possui uma aresta conectada com
    o vertice de referencia */
    for(int i = 0; i < tam_matrix; i++){
        if(lerMatriz(row,i) == 1){
            /* Caso exista aresta, é salvo o valor na EEPROM e o contador é incrementado */
            escreverIntEEPROM(pgm_read_word_near(POS_ARESTAS_VERTICE) + count, i);
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
* Nesta função, o boleano irá indicar que a aresta pode ser utilizada no caminho e não se ela é uma aresta de corte
*/
bool ehCaminhoValido(int start, int end, int n_arestas_vertice){
  /* Contador indicando a quantidade de arestas do vertice 'start' */
  int count = 0;
  /* Contabiliza as arestas de 'start' */
  for (int i = 0; i < n_arestas_vertice; i++){ 
    if(lerMatriz(start, i) == 1){
      count++;
    }
  }
  
  /* Caso o vertice 'start' possue apenas uma aresta conectada ao vertice 'end' e nenhuma outra, é utilizada 
  uma DFS para determinar se aquela aresta pode ser utilizada no caminho */
  if (count == 1){
    /* Salva a quantidade de vertices que podem ser alcançados apartir do vertice 'end' com a aresta de 'start' para 'end'*/
    int dfs_c_aresta = dfs(end);
    /* Remove a aresta da matriz */
    /*ALTER
    grafo_matriz[end][start] = 0;
    grafo_matriz[start][end] = 0;
    */
    escreveMatriz(0, start,end);
    

    /* Salva a quantidade de vertices que podem ser alcançados apartir do vertice 'end' caso a aresta seja removida */
    int dfs_s_aresta = dfs(end);

    /* Insere a aresta de volta a matriz */
    /*ALTER  
    grafo_matriz[end][start] = 1;
    grafo_matriz[start][end] = 1;
    */
    escreveMatriz(1, start,end);

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
    EEPROM.update(pos++, *p++);
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
    EEPROM.update(pos++, *p++);
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
    /*ALTER
    escreverIntEEPROM(pgm_read_word_near(POS_VETOR_VIZINHOS) + j, grafo_matriz[row][j]);
    */
    escreverIntEEPROM(pgm_read_word_near(POS_VETOR_VIZINHOS) + j, lerMatriz(row, j));
  }
}

/*
  Realiza uma busca em profundidade iterativa em uma estrutura de pilha
  Entrada: inteiro indicando o vertice de partida
  Saida: um inteiro que diz quantos vertices são acessiveis
*/
int dfs(int inicial){

    /* 
      Para realizar uma busca em profundidade de forma iterativa, declaramos uma pilha de inteiros 
      que ira guardar o index de cada vertice a ser visitado, um vetor de boleanos que indica os vertices 
      visitados e um vetor de inteiros com os vertices vizinhos ao vertice referencia 'inicial'.
      Os vetores de vertice visitados e o de vertices vizinhos, foi armazenado na memoria EEPROM.
      A DFS consiste em calcular quantos vertices podem ser acessados a partir do vetor inicial.
    */

    /* Pilha de execução, que irá somente ter a função de indicar quais vertices devem ser analisados*/
    int pilha[tam_matrix];
    /* Inicia o vetor da pilha com o valor '-1' em todas as suas posições, indicando que não existe 
    elementos na pilha que devem ser executados */
    iniciarVetor(pilha);

    /* Inicia o vetor boleano de vertices visitados com o valor 'false'. Cada index representa um vertice
    e o valor armazenado nele diz se esse vertice foi ou nao verificado durante a DFS */
    iniciarVetorBoolEEPROM(pgm_read_word_near(POS_VISITADO), false);

    /* O vertice 'inicial' enviado para a função é inserido na pilha de execução e é marcado true
    em seu index no vetor de vertices visitadas */
    empurrar(pilha, inicial);
    escreverBoolEEPROM(pgm_read_word_near(POS_VISITADO) + inicial, true);
  
    /* Aqui damos inicio a execução dos elementos da pilha, a condição de parada será quando a pilha 
    de execução estiver vazia */
    while(!ehVazio(pilha)){ 
        
      /* Variável que representa a vertice que será analisada. A escolha dessa vertice depende do ultimo 
      elemento que se encontra na pilha de execução*/
      int vertice;
      vertice = sacar(pilha);
      
      /* Salva na EEPROM a linha do grafo correspondente a vertice que está sendo analisada, isso significa
      que o vetor salvo na EEPROM irá possuir todas as arestas conectadas a esse vertice */
      copiarGrafoLinha(vertice);
      
      /* Itera sobre todos as vertices do grafo */
      for(int i = 0; i < tam_matrix; i++){
        /* Caso o vertice iterado no 'for' seja vizinho e não tenha sido visitado durante a DFS */
        if(lerIntEEPROM(pgm_read_word_near(POS_VETOR_VIZINHOS) + i) == 1 && !lerBoolEEPROM(pgm_read_word_near(POS_VISITADO) + i)){
            /* Insere o vertice não visitado na pilha de execução e escreve no vetor de vertices visitados 
            o valor 'true', indicando que este vertice ja foi visitado */
            empurrar(pilha, i);
            escreverBoolEEPROM(pgm_read_word_near(POS_VISITADO) + i, true);
        }
      }
    }

    /*
      Por fim, apos verificar todos os vertices que podem ser visitados apartir do vertice 'inicial', 
      realiza a contagem da quantidade de verticies que foram visitados e incrementa um contador
    */
    int count = 0;
    for(int i = 0; i < tam_matrix; i++){
      if(lerBoolEEPROM(pgm_read_word_near(POS_VISITADO) + i) == true){
          count++;
      }
    }

    /* Retorna o contador de vertices visitados */
    return count;
}

/*
  Inicia a pilha de execução da DFS com o valor -1, indicando que não existe elemento em todos 
  as posicoes.
  Entrada: um ponteiro para o vetor de inteiros da pilha
*/
void iniciarVetor(int *vector){
    for(int i=0; i < tam_matrix;i++){
        vector[i] = -1;
    }
}

/*
  Verifica se um vetor de pilha se encontra vazio. Cada elemento é considerado vazio se o valor
  '-1' estiver armazenado em sua posição.
  Entrada: um ponteiro para o vetor de inteiros da pilha
  Saida: um boleano indicando se a pilha esta ou nao vazia
*/
bool ehVazio(int *pilha){
    for(int i=0; i < tam_matrix; i++){
        if(pilha[i] != -1){
            return false;
        }    
    }
    return true;
}

/*
  Realiza um push() na pilha, inserindo o valor desejado no topo da pilha.
  Entrada: 
  'pilha'= um ponteiro para o vetor de inteiros da pilha
  'value' = valor a ser armazenado no topo da pilha
*/
void empurrar(int *pilha, int value){
  /* Itera sobre a pilha ate encontrar um elemento vazio, com '-1' armazenado */
  int i;
  for(i = 0; i < tam_matrix; i++){
      if(pilha[i] == -1){
          break;
      }
  }
  /* Salva no topo da pilha o valor recebido */
  pilha[i] = value;
}

/*
  Equivalente ao pop(), remove o elemento do topo da pilha.
  Entrada: um ponteiro para o vetor de inteiros da pilha
  Saida: um inteiro que é o elemento do topo da pilha
*/
int sacar(int *pilha){
    /* Itera sobre a pilha ate encontrar um elemento vazio, com '-1' armazenado */
    int i;
    for(i=0; i < tam_matrix; i++){
        if(pilha[i] == -1){
            break;
        }    
    }
    /* Armazena o valor no topo em uma variavel auxiliar, defini a posicao desse valor como vazia 
    e retorno a variavel auxiliar */
    int aux = pilha[i-1];
    pilha[i-1] = -1;
    return aux;
}

/*
  Calcula a quantidade de arestas presentes na matriz de adjacencia.
  Como o valor armazenado é equivalente a '1' ou a '0', e a matriz de adjacencia espelha 
  a parte inferior da matriz, basta somar todos os elementos da parte acima da diagonal
  Saida: um inteiro representando a quantidade de arestas presentes no grafo  
*/
int qtdArestas(){
  int arestas=0;
  /* O motive de 'j' iniciar em 'i', é para os elementos acessados, serem os da parte superior
  da matriz */
  for (int i = 0; i < tam_matrix; i++){
    for (int j = i; j < tam_matrix; j++){
      /*ALTER
      arestas+=grafo_matriz[i][j];
      */
      arestas+=lerMatriz(i,j);
    }
  }
  return arestas;
}

/*
  Utilitario para enviar pela Serial todos os elementos escritos na matriz
*/
void printGrafo(){
    Serial.println();
    for( int i =0 ; i < tam_matrix; i++){
        for( int j= 0; j < tam_matrix; j++){
            /*ALTER
            Serial.print(grafo_matriz[i][j]-0);
            */
            Serial.print(lerMatriz(i,j)-0);
            Serial.print(" ");
        }
        Serial.println();
    }
}