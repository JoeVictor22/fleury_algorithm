#include <stdbool.h>
#include <stdio.h>
#include "definitions.h"
/*--------------------------------------------------------------------------------------------------------*/
/*         Utilitarios para ser utilizado no Grafo necessario para a aplicação                            */
/*--------------------------------------------------------------------------------------------------------*/

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