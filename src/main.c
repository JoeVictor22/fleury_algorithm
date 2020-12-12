#include <stdio.h>
#include <stdbool.h>
#include "dfs.h"



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
/*
Calcula a quantidade de arestas de dado vertice
*/
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
/*
Realiza a copia de um grafo
*/
void copy_graph(int size, int (*copy)[size], int (*orig)[size]){   
    for( int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            copy[i][j] = orig[i][j];
        }
    }
}
/*------------------------------------------------------------------------------------------------*/

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



/*------------------------------------------------------------------------------------------------*/
/*
TODO, raynan explica melhor isso aq
*/
bool isValidNextEdge(int start, int end, int *adjacentes, int n_arestas_vertice,
                     int size, int (*grafo)[size]){
  
  int count = 0;
  for (int i = 0; i < n_arestas_vertice; i++){ 
    if(grafo[start][i] == 1){
      count++;
    }
  }

  printf("\ncount_aresta: %d", count);
  print_row(n_arestas_vertice, adjacentes);
  if (count == 1){
    int dfs_c_aresta = dfs(size, grafo, end);
  

    // TODO tentar remover a copia de grafos
    // copy of graph without aresta
    int copy[size][size];
    copy_graph(size, copy, grafo);
    
    copy[start][end] = 0;
    copy[end][start] = 0;

    int dfs_s_aresta = dfs(size, copy, end);
    printf("\ndfs c aresta: %d", dfs_c_aresta);
    printf("\ndfs s aresta: %d", dfs_s_aresta);

    return (dfs_c_aresta > dfs_s_aresta) ? false : true;

  }else {
    return true;
  }

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

    printf("\n\nstart:%d -> n_vertices: %d", start, n_arestas_vertice);
    // iterando sobre as arestas
    // arestas_vertice[0] = 3, 5, 7 o nome de uma das vertices
    int i;
    for (i = 0; i < n_arestas_vertice; i++){
       
      v = arestas_vertice[i]; 
      printf("\ntalvez eu va para : %d", v);
      // If edge u-v is not removed and it's a a valid next edge 
      if (grafo[start][v] == 1 && isValidNextEdge(start, v, arestas_vertice, n_arestas_vertice, size, grafo)){ 
      //if (grafo[start][v] == 1){ 
        printf("\nENTROU NA CONDICIONAL");     
        
        grafo[start][v] = 0;
        grafo[v][start] = 0; 
        printf("\nPASSO: %d -> %d",start, v);
        start = v;    
        caminho[vertices_visitados] = start; 
        vertices_visitados++;
        break;
      }
    } 
    if(i >= n_arestas_vertice){
        printf("\nEXCECAO apos o FOR %d %d", i, n_arestas_vertice);     

        grafo[start][v] = 0;
        grafo[v][start] = 0; 
        printf("\nPASSO: %d -> %d",start, v);
        start = v;
        caminho[vertices_visitados] = start; 
        vertices_visitados++;
    }

    print_row(n_arestas_vertice, arestas_vertice);
    
    if(n_arestas_vertice == 0){
      break;
    }
  }
  printf("n_vertices_visitados %d\n", vertices_visitados);
  print_graph(size, grafo);

  printf("\nCAMINHO");
  for( int i = 0 ; i < vertices_visitados; i++){
    printf("-> %d ", caminho[i]);
  }
} 
/*------------------------------------------------------------------------------------------------*/

/*
Preenche dada matriz com um valor recebido
*/
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


  // validar se é INT
  int matrix_size;
  printf("digite o tamanho da matriz tam x tam\n");
  scanf("%d", &matrix_size);


  // permitir inserir a matriz na forma de pares ou item por item
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

  printf("\nstart: %d\n", start);
  
  fleury(matrix_size, grafo, start);

  
  /*TODO  na saida informar algumas caracteristicas do grafo
    verificar se forma ciclo, se é euleriano ou se não é 
  */

  return 0;
}