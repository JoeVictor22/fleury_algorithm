#include <stdio.h>
#include <stdbool.h>
#include "dfs.h"
#include "grafo.h"

#define MAX_SIZE 100
#define MIN_SIZE 2


/*------------------------------------------------------------------------------------------------*/
/*
Busca o vertice mais indicado para dar inicio no caminho
busca um vertice com degrau impar, caso nao exista nenhum, retorna um vertice qualquer do grafo
*/
int buscarInicial(int numVert, int (*grafo)[numVert]){

  int degraus = 0;
  
  for(int i=0; i < numVert; i++){
    degraus = 0;
    for(int j = 0; j < numVert; j++){
      if (grafo[i][j] == 1){
        degraus++;
      }
    }
    if((degraus % 2 ) == 1 ){
      //*** MAYBE ***//
      //*** a gente podia guardar o vertice com o maior numero de arestas para dar ser nosso vertice inicial, e nao pegar o primeiro impar encontrado ***//
      return i;
    }
  }
  //*** MAYBE ***//
  //*** a gente pode retornar um random para a trilha iniciar de qualquer um dos valores***//
  
  return 0;
}
/*------------------------------------------------------------------------------------------------*/
/*
Busca um caminho ou ciclo euleriano no grafo e avalia algumas caracteristicas do grafo
*/
void fleury(int size, int (*grafo)[size], int start){
  int vertices_visitados = 0;
  int v = start;
  int caminho[100];
  caminho[vertices_visitados] = start; 
  vertices_visitados++;
  while(true){
    // capturar primeira aresta disponivel dessa linha

    // vetor com as arestas de dado vertie inicial
    int arestas_vertice[size];
    // quantidade de arestas naqule vertice
    int n_arestas_vertice = 0;
    n_arestas_vertice = countArestas(size, arestas_vertice, grafo, start);

    // iterando sobre as arestas
    int i;
    for (i = 0; i < n_arestas_vertice; i++){
       
      v = arestas_vertice[i]; 
      // If edge u-v is not removed and it's a a valid next edge 
      if (grafo[start][v] == 1 && ehArestaCorte(start, v, arestas_vertice, n_arestas_vertice, size, grafo)){ 
        
        grafo[start][v] = 0;
        grafo[v][start] = 0; 
        start = v;    
        caminho[vertices_visitados] = start; 
        vertices_visitados++;
        break;
      }
    } 
    if(i >= n_arestas_vertice){

        grafo[start][v] = 0;
        grafo[v][start] = 0; 
        start = v;
        caminho[vertices_visitados] = start; 
        vertices_visitados++;
    }

    if(n_arestas_vertice == 0){
      break;
    }
  }

  // ultimo vertice é visitado na ultima iteracao
  vertices_visitados--;
  printGrafo(size, grafo);

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
/*------------------------------------------------------------------------------------------------*/

/*
Preenche dada matriz com um valor recebido
*/
void fillMatrix( int size, int value, int (*vector)[size]){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      vector[i][j] = value;
    }
  }
}

int main(){

  int matrix_size;
  printf("Digite o tamanho da matriz: TxT\n");
  scanf("%d", &matrix_size);
  
  if( matrix_size > MAX_SIZE || matrix_size < MIN_SIZE){
    printf("O tamanho máximo aceitado de linhas/colunas é de %d e o minimo é %d\n", MAX_SIZE, MAX_SIZE, MIN_SIZE);
    return 0;
  }

  int grafo[matrix_size][matrix_size];
  fillMatrix(matrix_size, 0, grafo);
  
  int n_arestas;
  char option;
  getchar();
  printf("Digite 'A' para inserir uma matriz de adjacencia ou digite 'B' para inserir as arestas do grafo.\n");
  scanf("%c", &option );
  switch(option){
    case 'A':
    case 'a':
      printf("Insira os %d valores da matriz de adjacencia, cada valor inserido deve ser 0 ou 1 e devem ser separados por espaço!\n", (matrix_size * matrix_size));
      // TODO validar se input é 0 ou 1
      for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++ ){
          int temp;
          scanf("%d", &temp);
          if(temp != 0 && temp != 1){
            printf("Valor invalido");
            j--;
          }else{
            grafo[i][j] = temp;
          }

        }
      }
      break;
    case 'B':
    case 'b':
      // TODO validar se o numero de arestas é int
      // TODO validar se input é 0 ou 1
      printf("Digite quantas arestas o grafo possui!\n");
      scanf("%d", &n_arestas);
      getchar();

      printf("Digite os %d pares, cada par deve ser inserido no formato 'x-y'.\n", n_arestas);
      for(int i = 0; i < n_arestas; i++){
        int x,y;
        scanf("%d-%d", &x, &y);
        getchar();

        if(x >= matrix_size || y >= matrix_size ){
          printf("Valor inválido");
          return 0;
        }

        addAresta(matrix_size, x, y, grafo);
      }
      break;
    default:
      printf("Opção inválida\n");
      return 0;
      break;
  }

  printf("\nMATRIZ INSERIDA\n");
  for(int i = 0; i < matrix_size; i++){
    for(int j = 0; j < matrix_size; j++ ){
      printf("%d ", grafo[i][j]);
    }
    printf("\n");
  }

  int start = buscarInicial(matrix_size, grafo); 

  fleury(matrix_size, grafo, start);
  
  /*TODO  na saida informar algumas caracteristicas do grafo
    verificar se forma ciclo, se é euleriano ou se não é 
  */

  return 0;
}