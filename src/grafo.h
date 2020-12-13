#include <stdbool.h>
#include <stdio.h>
#include "dfs.h"

void copiarGrafo(int size, int (*copy)[size], int (*orig)[size]){   
    for( int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            copy[i][j] = orig[i][j];
        }
    }
}

void copiarGrafoLinha(int size, int *copy, int (*orig)[size], int row){   
    for(int j = 0; j < size; j++){
        copy[j] = orig[row][j];
    }
}

void printGrafo(int size, int (*grafo)[size]){
    printf("\nGRAFO:\n");
    for( int i =0 ; i < size; i++){
        for( int j= 0; j < size; j++){
            printf("%d ", grafo[i][j]);
        }
        printf("\n");
    }
}

void printLinha(int size, int *vector){
    printf("\nLINHA:\n");
    for(int i = 0; i < size; i++){
        printf("%d ", vector[i]);
    }
    printf("\n");
}


/*
Recebe uma aresta na forma de par e adiciona a matriz de adjacencia
*/
void addAresta( int size, int i, int j, int (*matriz)[size]){
  matriz[i][j] = 1;
  matriz[j][i] = 1;
}

/*------------------------------------------------------------------------------------------------*/
/*
Valida a Aresta do grafo a ser utilizada
*/
bool ehArestaCorte(int start, int end, int *adjacentes, int n_arestas_vertice,
                     int size, int (*grafo)[size]){
  //Contabiliza a quantidade de ligações que o Vertice possui
  int count = 0;
  for (int i = 0; i < n_arestas_vertice; i++){ 
    if(grafo[start][i] == 1){
      count++;
    }
  }


  //Caso o vertice só possua uma Aresta é realizado uma DFS a fim de reconhecer se ele é uma "Aresta de corte"
  if (count == 1){
    int dfs_c_aresta = dfs(size, grafo, end);
     //*** TODO tentar remover a copia de grafos***//
    //*** copy of graph without aresta         ***//
    int copy[size][size];
    copiarGrafo(size, copy, grafo);
    
    copy[start][end] = 0;
    copy[end][start] = 0;

    int dfs_s_aresta = dfs(size, copy, end);

	// Ao comparar o resultado retornado pelas DFS's é definido se a aresta será usada.
    return (dfs_c_aresta > dfs_s_aresta) ? false : true;
  }else {
    return true;
  }

}

/*------------------------------------------------------------------------------------------------*/
/*
Calcula a quantidade de arestas do grafo
*/
int qtdArestas(int size, int (*grafo)[size]){
  int arestas=0;
  for (int i=0;i<size;i++){
    for (int j=i;j<size;j++){
      arestas+=grafo[i][j];
    }
  }
  return arestas;
}


/*
Retorna a quantidade de arestas de um vertice e salva essas vertices em um vetor 
*/
int countArestas(int size, int *arestas, int (*grafo)[size], int row){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(grafo[row][i] == 1){
            arestas[count] = i;
            count++;
        }
    }
    return count;
}

