#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// max size para o numero de vertices
#define MAX_SIZE 37
// min size para o numero de vertices
#define MIN_SIZE 2
// max size para o caminho percorrido no pior caso possivel: (37 * 36)/2
#define MAX_CAMINHO 700
// no pior caso, o algoritmo deve usar cerca de 2069 inteiros

// matriz utilizada para a computação dos dados
int grafo_matriz[MAX_SIZE][MAX_SIZE];
int matriz_copia_teste[MAX_SIZE];

/*
Implementação Iterativa do algoritimo de Fleury com busca em profundidade

Copyright © 2020 by Joel Victor Castro Galvão, Raynan Serafim de Souza

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/*--------------------------------------------------------------------------------*/
/*A main do programa realiza diversas validações da entrada do usuario, em seguida*/
/*mostra diversas opções para a geração de uma trilha euleriana                   */
/*As trilhas geradas podem não ser eficientes, o algoritmo apenas busca obter uma */
/*trilha euleriana sem se preocupar com otimização do caminho                     */
/*Uma maneira de validar sua saída, é checar se a matriz de adjacencia se encontra*/
/*totalmente vazia após a execução do algoritmo, isso indica que todos os vertices*/
/*foram percorridos e destruidos adequadamentes                                   */
/*Apos o usuario digitar sua entrada, a matriz gerada é printada, e apos o fim do */
/*programa, a matriz com as arestas percorridas é printada, assim como o caminho  */
/*ou uma resposta indicando que não foi possivel encontrar uma trilha euleriana   */
/*--------------------------------------------------------------------------------*/
/*Primeira entrada: Quantidade de Vertices do grafo.                              */
/*Segunda entrada: A para escolher inserir uma matriz de adjacencia ou B para     */
/*Segunda entrada: Existem tambem duas opcoes, C e D, para casos de teste         */
/*pares de arestas.                                                               */
/*Terceira entrada: Caso A, matriz de adjacencia com N*N elementos.               */
/*Terceira entrada: Caso B, número de Arestas e pares de Arestas no formato 'x-y'.*/
/*Terceira entrada: Caso C, essa opção ira gerar uma matriz do tamanho informado .*/
/*Terceira entrada: Caso D, essa opção ira realizar uma serie de testes          .*/
/*--------------------------------------------------------------------------------*/











void copiarGrafoLinha(int size, int *copy, int row);   
void printGrafo(int size);
void printLinha(int size, int *vector);
void addAresta( int size, int i, int j);
bool ehArestaCorte(int start, int end, int *adjacentes, int n_arestas_vertice, int size);
int qtdArestas(int size);
int countArestas(int size, int *arestas, int row);
void fillMatrix( int size, int value);




/*--------------------------------------------PILHA-------------------------------------------------------*/
void iniciarVetor(int *vector, int size){
    for(int i=0; i < size;i++){
        vector[i] = -1;
    }
}

// Verifica se a pilha esta preenchida com somente valores -1
// Caso verdade a pilha se encontra vazia, retorna TRUE.
bool ehVazio(int *pilha, int size){
    for(int i=0; i < size; i++){
        if(pilha[i] != -1){
            return false;
        }    
    }
    return true;
}

// Insere um valor no topo da pilha
void empurrar(int *pilha, int size, int value){
    int i;
    for(i = 0; i < size; i++){
        if(pilha[i] == -1){
            break;
        }
    }
    pilha[i] = value;
}

// Saca o valor que se encontra no topo da pilha
int sacar(int *pilha,int size){
    int i;
    for(i=0; i < size; i++){
        if(pilha[i] == -1){
            break;
        }    
    }
    int aux = pilha[i-1];
    pilha[i-1] = -1;
    return aux;
}
/*--------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------DFS-------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------*/
/*Implementação iterativa do DFS modificada para retornar a quantidade de arestas acessiveis dado um      */
/*vertice inicial                                                                                         */
/*--------------------------------------------------------------------------------------------------------*/
int dfs(int size, int inicial){
    // Pilha de proximos a entrar na DFS
    int pilha[size];
    iniciarVetor(pilha, size);
    
	// Vetor boolenao armazenando vertices que são possiveis ser acessados
    bool visitado[size];
    for(int i = 0; i < size; i++){
        visitado[i] = false;
    }

	// A primeira vertice na DFS é o inicial passado como argumento
    empurrar(pilha, size, inicial);
    visitado[inicial] = true;
	
	// Realiza verificações até a pilha estar vazia
    while(!ehVazio(pilha, size)){ 
        
		// Vertice é a vertice onde ira ser feito a DFS
        int vertice;
        vertice = sacar(pilha, size);
				
        // Recebe um vetor que é a linha da Matriz correspondente ao Vertice a ser feita a DFS
        int vetorVizinhos[size];
        copiarGrafoLinha(size, vetorVizinhos, vertice);
        
		// "i" itera sobre o vetor e marca os vizinhos ao Vertice utilizado
        for(int i=0; i < size; i++){
			// Caso o vertice seja vizinho, (==1), e ainda não tenha sido marcado como visitado
			// O vizinho será colocado na pilha para verificar os seus proprios vizinhos e marcado como visitado
            if(vetorVizinhos[i] == 1 && !visitado[i]){
                empurrar(pilha, size, i);
                visitado[i] = true;
            }
        }
    }

	// Realiza uma contagem de quantos vertices é possivel atingir a partir do inicial
    int count = 0;
    for(int i = 0; i < size; i++){
        if(visitado[i] == true){
            count++;
        }
    }

	// Retornar a contagem para comparações
    return count;
}




/*--------------------------------------------------------------------------------------------------------*/

// Copia somente uma linha do Grafo para um vetor
void copiarGrafoLinha(int size, int *copy, int row){   
    for(int j = 0; j < size; j++){
        copy[j] = grafo_matriz[row][j];
    }
}
/*--------------------------------------------------------------------------------------------------------*/

// Printa o Grafo em sua integridade
void printGrafo(int size){
    printf("\nGRAFO:\n");
    for( int i =0 ; i < size; i++){
        for( int j= 0; j < size; j++){
            printf("%d ", grafo_matriz[i][j]);
        }
        printf("\n");
    }
}
/*--------------------------------------------------------------------------------------------------------*/

/*
Utilitario para gerar uma matriz, caso o tamanho seja impar ela sera euleriana
*/
void create(int size){
  for(int i =0 ; i < size; i++){
    for( int j=0; j < size; j++){
      if (j != i){
        grafo_matriz[i][j] = 1;
      }
    }
  }
}
/*--------------------------------------------------------------------------------------------------------*/
// Printa somente uma linha do Grafo
void printLinha(int size, int *vector){
    printf("\nLINHA:\n");
    for(int i = 0; i < size; i++){
        printf("%d ", vector[i]);
    }
    printf("\n");
}
/*--------------------------------------------------------------------------------------------------------*/

// Recebe uma aresta na forma de par e adiciona a matriz de adjacencia
void addAresta( int size, int i, int j){
  grafo_matriz[i][j] = 1;
  grafo_matriz[j][i] = 1;
}
/*--------------------------------------------------------------------------------------------------------*/

// Calcula a quantidade de arestas presentes no grafo
int qtdArestas(int size){
  int arestas=0;
  for (int i=0;i<size;i++){
    for (int j=i;j<size;j++){
      arestas+=grafo_matriz[i][j];
    }
  }
  return arestas;
}
/*--------------------------------------------------------------------------------------------------------*/

// Retorna a quantidade de arestas de um vertice e salva essas vertices em um vetor 
int countArestas(int size, int *arestas, int row){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(grafo_matriz[row][i] == 1){
            arestas[count] = i;
            count++;
        }
    }
    return count;
}
/*--------------------------------------------------------------------------------------------------------*/

//Preenche dada matriz com um valor recebido
void fillMatrix( int size, int value){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      grafo_matriz[i][j] = value;
    }
  }
}
/*--------------------------------------------------------------------------------*/

// Valida a Aresta do grafo a ser utilizada
bool ehArestaCorte(int start, int end, int *adjacentes, int n_arestas_vertice,
                   int size){
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
    int dfs_c_aresta = dfs(size, end);
    
    grafo_matriz[end][start] = 0;
    grafo_matriz[start][end] = 0;

  	// Salva a quantidade de Vertices acessiveis sem a Aresta presente no grafo
    int dfs_s_aresta = dfs(size, end);

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


// Busca o vertice mais indicado para dar inicio no caminho
// Busca um vertice com grau ímpar, caso não exista nenhum, retorna um vertice qualquer do grafo
int buscarInicial(int numVert){
  int grau = 0;
  for(int i=0; i < numVert; i++){
    grau = 0;
    for(int j = 0; j < numVert; j++){
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
/*--------------------------------------------------------------------------------*/

// Busca um caminho ou ciclo euleriano no grafo assim como avalia algumas caracteristicas do grafo
void fleury(int size, int start){
  // Contagem de vertices visitados
  int vertices_visitados = 0;
  int v = start;
  
  // Calcula o tamanho maximo do caminho que pode ser percorrido
  int max_caminho_size = ((size * (size-1))/2);
  if(max_caminho_size > MAX_CAMINHO){
    max_caminho_size = MAX_CAMINHO;
  }

  // Caminho feito no grafo a ser impresso
  int caminho[max_caminho_size];
  caminho[vertices_visitados] = start; 
  vertices_visitados++;
  
  while(true){
    // Cria um vetor com as arestas dado o vertice inicial
    int arestas_vertice[size];
	
    // Quantidade de arestas naquele vertice
    int n_arestas_vertice = 0;
    n_arestas_vertice = countArestas(size, arestas_vertice, start);

    // Itera sobre as arestas
    int i;
    for (i = 0; i < n_arestas_vertice; i++){
    // V recebe a Aresta 
	  v = arestas_vertice[i];
	  
      // Verfica se a posição no Grafo é valida e se a Aresta não é de corte
      if (grafo_matriz[start][v] == 1 && ehArestaCorte(start, v, arestas_vertice, n_arestas_vertice, size)){
	    	// Caso a Aresta seja validada ela é removida do Grafo
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0;
		
	    	// O ponto final da Aresta é se torna o ponto inicial para as proximas verificações
        start = v;  

	    	// Guarda o Vertice para printar caminho
        caminho[vertices_visitados] = start; 
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
        caminho[vertices_visitados] = start; 
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
  printGrafo(size);

  // Um Grafo contem um caminho Euleriano se somente se ele percore todas as arestas contidas nele.
  if (qtdArestas(size) == 0){
    printf("\nCAMINHO ");
    for( int i = 0 ; i < vertices_visitados; i++){
      printf("-> %d ", caminho[i]);
    }
  }else{
    printf("\nO grafo não possui trilha euleriana!");
  }

  printf("\n");
} 


/*--------------------------------------------------------------------------------------------------------*/

// Função para teste exaustivo da aplicação
void teste(){
  printf("\n\nTESTES\n"); 
  int i;
  for( i = MIN_SIZE; i <= MAX_SIZE; i++){
    printf("\n\nSIZE: %d", i);
    fillMatrix(i, 0);

    create(i);
    // Busca um vertice para iniciar a verificação do grafo
    int start = buscarInicial(i); 

    // Trabalha para encontrar um caminho Euleriano no grafo
    fleury(i, start);
  } 
}
/*--------------------------------------------MAIN-----------------------------------------------------*/
int main(){
  // Recebe o tamanho da Matriz que sera trabalhada
  int matrix_size;
  printf("Digite o tamanho da matriz: TxT\n");
  scanf("%d", &matrix_size);
  
  // Valida se o valor recebido esta dentro dos limites
  if( matrix_size > MAX_SIZE || matrix_size < MIN_SIZE){
    printf("O tamanho máximo aceitado de linhas/colunas é de %d e o minimo é %d\n", MAX_SIZE, MIN_SIZE);
    exit(0);
  }

  // Cria o grafo e o preenche com 0
  //int grafo[matrix_size][matrix_size];
  fillMatrix(matrix_size, 0);
  
  // Recebe o modo de entradas desejado
  int n_arestas;
  char option;
  getchar();
  printf("Digite 'A' para inserir uma matriz de adjacencia\nDigite 'B' para inserir as arestas do grafo\nDigite 'C' para criar uma matriz automaticamente\nDigite 'D' para realizar casos de teste\n");
  scanf("%c", &option );
  switch(option){
    case 'A':
    case 'a':
      printf("Insira os %d valores da matriz de adjacencia, cada valor inserido deve ser 0 ou 1 e devem ser separados por espaço!\n", (matrix_size * matrix_size));
      for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++ ){
          // Recebe a entrada do usuario
	    	  int temp;
          scanf("%d", &temp);
		  
		      // Valida a entrada do usuario, os valores podem ser somente 0 ou 1
          if(temp != 0 && temp != 1){
            printf("Valor invalido, insira 1 ou 0!\n");
            getchar();
            j--;
          }else{
            grafo_matriz[i][j] = temp;
          }
        }
      }
      break;
    case 'B':
    case 'b':
	   // Recebe a quantidade de arestas no grafo
      printf("Digite quantas arestas o grafo possui!\n");
      scanf("%d", &n_arestas);
      getchar();
      
	    // Valida a entrada do usuario, o grafo deve possuir no minimo 1 Aresta
      if(n_arestas <= 0){
        printf("O número de arestas digitado é invalido!\n");
        exit(0);
      }

	    // Recebe os pares de arestas no formato x-y
      printf("Digite os %d pares, cada par deve ser inserido no formato 'x-y'.\n", n_arestas);
      for(int i = 0; i < n_arestas; i++){
        int x,y;
        scanf("%d-%d", &x, &y);
        printf("\nPAR: %d -> %d\n", x, y);
		
	    	// Valida se a Aresta esta contida no Grafo
        if(x < 0 || y < 0 || x >= matrix_size || y >= matrix_size ){
          printf("\nValor inválido, digite valores de 0 ate %d\n", matrix_size-1);
          getchar();
          i--;
        }else{
          addAresta(matrix_size, x, y);
        }
      }
      break;
    // Cria uma matriz com arestas conectadas a todos os vertices, se o tamanho for impar sera gerado um grafo euleriano
    case 'C':
    case 'c':
      create(matrix_size);
      break;
    // Realiza um teste sobre todos os grafos possiveis ate o tamanho max permitido pelo algoritimo, gerando grafos com a mesma estrategia do caso 'C'
    case 'D':
    case 'd':
      teste();
      exit(0);
      break;
  	// Valida o caso de entrada
    default:
      printf("Opção inválida\n");
      exit(0);
      break;
  }

  // Confirma que a matriz foi inserida em sua integridade e printa para o usuario a mesma
  printf("\nMATRIZ INSERIDA\n");
  for(int i = 0; i < matrix_size; i++){
    for(int j = 0; j < matrix_size; j++ ){
      printf("%d ", grafo_matriz[i][j]);
    }
    printf("\n");
  }

  // Busca um vertice para iniciar a verificação do grafo
  int start = buscarInicial(matrix_size); 

  // Trabalha para encontrar um caminho Euleriano no grafo
  fleury(matrix_size, start);

  return 0;
}



/*-----------------------------------------------------------------------------------------------------*/