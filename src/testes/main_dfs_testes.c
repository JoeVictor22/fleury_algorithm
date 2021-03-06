#include <stdio.h>
#include <stdbool.h>
#include "dfs.h"



/*------------------------------------------------------------------------------------------------*/
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
      /* MAYBE
      a gente podia guardar o vertice com o maior numero de arestas para dar ser nosso vertice inicial, e nao pegar o primeiro impar encontrado
      */
      return i;
    }
  }
  /* MAYBE
  a gente pode retornar um random para a trilha iniciar de qualquer um dos valores 
  */
  return 0;

}   
/*------------------------------------------------------------------------------------------------*/
int qtdArestas(int numVert, int (*grafo)[numVert]){
  printf("vertice=%d\n",numVert);
  int arestas=0;
  for (int i=0;i<numVert;i++){
    for (int j=i;j<numVert;j++){
      arestas+=grafo[i][j];
    }
  }
  printf("aresta=%d\n",arestas);
  return arestas;
}
/*------------------------------------------------------------------------------------------------*/

void copy_graph(int size, int (*copy)[size], int (*orig)[size]){   
    for( int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            copy[i][j] = orig[i][j];
        }
    }
}
/*------------------------------------------------------------------------------------------------*/

/*
retorna a quantidade de arestas de um vertice e anexa a um vetor as arestas 
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



/*------------------------------------------------------------------------------------------------*/
bool isValidNextEdge(int start, int end, int *adjacentes, int n_arestas_vertice,
                     int size, int (*grafo)[size]){
  
  int count = 0;
  for (int i = 0; i < n_arestas_vertice; i++){ 
    if(grafo[start][i] == 1){
      count++;
    }
  }

  if (count == 1){
    return false;
  }

  int dfs_c_aresta = dfs(size, grafo, end);
  
  // copy of graph without aresta
  int copy[size][size];
  copy_graph(size, copy, grafo);
  
  copy[start][end] = 0;
  copy[end][start] = 0;

  int dfs_s_aresta = dfs(size, copy, end);
  
  return ((dfs_c_aresta > dfs_s_aresta) ? true : false);

}
/*------------------------------------------------------------------------------------------------*/

void fleury(int size, int (*grafo)[size], int start){
  int vertices_visitados = 0;
  int v = 0;
  while(true){
    // capturar primeira aresta disponivel dessa linha

    // vetor com as arestas de dado vertie inicial
    int arestas_vertice[size];
    // quantidade de arestas naqule vertice
    int n_arestas_vertice = 0;
    n_arestas_vertice = countArestas(size, arestas_vertice, grafo, start);

    printf("n_vertices: %d\n", n_arestas_vertice);
    // iterando sobre as arestas
    // arestas_vertice[0] = 3, 5, 7 o nome de uma das vertices
    for (int i = 0; i < n_arestas_vertice; i++){ 
      v = arestas_vertice[i]; 
      // If edge u-v is not removed and it's a a valid next edge 
      if (grafo[start][v] == 1 && isValidNextEdge(start, v, arestas_vertice, n_arestas_vertice, size, grafo)){ 
        printf("PASSO: %d -> %d\n",start, v);     
        vertices_visitados++;
        grafo[start][v] = 0;
        grafo[v][start] = 0;     
        break;
      } 
    } 
    start = v;

    if(n_arestas_vertice == 0){
      break;
    }
    printf("while(true)");
  }
  printf("n_vertices_visitados %d\n", vertices_visitados);
} 
/*------------------------------------------------------------------------------------------------*/


void fill_matrix( int size, int value, int (*vector)[size]){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      vector[i][j] = value;
    }
  }
}

int main(){

    /*
    0, 1, 0, 1, 1, 1, 
    1, 0, 1, 0, 1, 0, 
    0, 1, 0, 1, 1, 0, 
    1, 0, 1, 0, 1, 1, 
    1, 1, 1, 1, 0, 0, 
    1, 0, 0, 1, 0, 0,
    */ 
    // fill matrix with 0

    int matrix_size;
    printf("digite o tamanho da matriz tam x tam\n");
    scanf("%d", &matrix_size);

    int grafo[matrix_size][matrix_size];
    fill_matrix(matrix_size, 0, grafo);

    printf("digita todos os %d itens da matriz\n", (matrix_size * matrix_size));
    // validar se input é 0 ou 1
    for(int i = 0; i < matrix_size; i++){
    for(int j = 0; j < matrix_size; j++ ){
        scanf("%d", &grafo[i][j]);
    }
    }

    printf("\nMATRIZ\n");
    for(int i = 0; i < matrix_size; i++){
    for(int j = 0; j < matrix_size; j++ ){
        printf("%d ", grafo[i][j]);
    }
    printf("\n");
    }
    //0 1 0 1 1 1 1 0 1 0 1 0 0 1 0 1 1 0 1 0 1 0 1 1 1 1 1 1 0 0 1 0 0 1 0 0

    int start = buscarInicial(matrix_size, grafo); 

    //printf("\nstart: %d\n", start);

    int resp = dfs(matrix_size, grafo, 0);

    printf("DFS: %d\n", resp);

    return 0;
}