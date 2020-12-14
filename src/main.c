#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "dfs.h"
#include "grafo.h"

// max size para o numero de vertices
#define MAX_SIZE 45
// min size para o numero de vertices
#define MIN_SIZE 2
// max size para o caminho percorrido no pior caso possivel: (45 * 44)/2
#define MAX_CAMINHO 2000
// no pior caso, o algoritmo deve usar cerca de 3000 inteiros

/*--------------------------------------------------------------------------------*/
/*Realiza diversas validações e recebe a entrada do usuario                       */
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


// Busca o vertice mais indicado para dar inicio no caminho
// Busca um vertice com grau ímpar, caso não exista nenhum, retorna um vertice qualquer do grafo
int buscarInicial(int numVert, int (*grafo)[numVert]){
  int grau = 0;
  for(int i=0; i < numVert; i++){
    grau = 0;
    for(int j = 0; j < numVert; j++){
      if (grafo[i][j] == 1){
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
void fleury(int size, int (*grafo)[size], int start){
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
    n_arestas_vertice = countArestas(size, arestas_vertice, grafo, start);

    // Itera sobre as arestas
    int i;
    for (i = 0; i < n_arestas_vertice; i++){
    // V recebe a Aresta 
	  v = arestas_vertice[i];
	  
      // Verfica se a posição no Grafo é valida e se a Aresta não é de corte
      if (grafo[start][v] == 1 && ehArestaCorte(start, v, arestas_vertice, n_arestas_vertice, size, grafo)){
	    	// Caso a Aresta seja validada ela é removida do Grafo
        grafo[start][v] = 0;
        grafo[v][start] = 0;
		
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
        grafo[start][v] = 0;
        grafo[v][start] = 0; 
		
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
  printGrafo(size, grafo);

  // Um Grafo contem um caminho Euleriano se somente se ele percore todas as arestas contidas nele.
  if (qtdArestas(size, grafo) == 0){
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
  int grafo[MAX_SIZE][MAX_SIZE];
 
  int i;
  for( i = MIN_SIZE; i <= MAX_SIZE; i++){
    printf("\n\nSIZE: %d", i);
    fillMatrix(i, 0, grafo);

    create(i, grafo);
    // Busca um vertice para iniciar a verificação do grafo
    int start = buscarInicial(i, grafo); 

    // Trabalha para encontrar um caminho Euleriano no grafo
    fleury(i, grafo, start);
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
    printf("O tamanho máximo aceitado de linhas/colunas é de %d e o minimo é %d\n", MAX_SIZE, MAX_SIZE, MIN_SIZE);
    exit(0);
  }

  // Cria o grafo e o preenche com 0
  int grafo[matrix_size][matrix_size];
  fillMatrix(matrix_size, 0, grafo);
  
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
            grafo[i][j] = temp;
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
          addAresta(matrix_size, x, y, grafo);
        }
      }
      break;
    // Cria uma matriz com arestas conectadas a todos os vertices, se o tamanho for impar sera gerado um grafo euleriano
    case 'C':
    case 'c':
      create(matrix_size, grafo);
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
      printf("%d ", grafo[i][j]);
    }
    printf("\n");
  }

  // Busca um vertice para iniciar a verificação do grafo
  int start = buscarInicial(matrix_size, grafo); 

  // Trabalha para encontrar um caminho Euleriano no grafo
  fleury(matrix_size, grafo, start);

  return 0;
}



/*-----------------------------------------------------------------------------------------------------*/