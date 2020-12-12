#include <stdio.h>
#include <stdbool.h>
#include "bfs.h"

#define NODE 6

int grafo[NODE][NODE];


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

void fleury(int size, int (*grafo)[size], int start){
    bool flag = true;
    int vertices = size;
    int v_ini = start;

    // VARs para registrar caminho percorrido
    int caminho[size+1];
    caminho[0] = v_ini;
    int passos = 0;

    bool da_ok = false;
    while(flag){
        flag = true;

        int arestas_vertice[size];
        int n_arestas_vertice = 0;
        n_arestas_vertice = countArestas(size, arestas_vertice, grafo, v_ini);

        for (int aresta = 0 ; aresta < n_arestas_vertice; aresta++){
               //copy graph and remove bridge
                //test bfs in graph
                int copy[size][size];
                copy_graph(size, copy, grafo);
                copy[v_ini][arestas_vertice[aresta]] = 0;
                copy[arestas_vertice[aresta]][v_ini] = 0; 
                
                /* 
                    vou ter q verificar se ao remover esse ponto, tds os outros serao alcancados
                */
                da_ok = false;
            for (int i = 0; i < size; i++){      

                //int **grafo, int start, int end, int size
                da_ok = bfs(size, copy, v_ini, i);
                //printf("BFS: %c - %d - %d = %d = %d\n", (da_ok ? 'S': 'N' ), aresta, i, n_arestas_vertice, size);
                if (!da_ok){
                    i = size;

                }   
                /* se o numero de arestas nesse ponto é igual a 1, ou se ao remover a ponte
                ainda é possivel chegar no fim, usar esse vertice
                */
                /* (esquece, acho q ajeitei) -> verificar se o n_vertices == 1 n vai acarretar em erro, ja q qnd houvesse 2
                vertices, n ira haver verificacao de bfs
                */

            }

            passos++;
            caminho[passos] = arestas_vertice[aresta];
            printf("%d -> %d\n", v_ini, arestas_vertice[aresta]);
            grafo[v_ini][arestas_vertice[aresta]] = 0;
            grafo[arestas_vertice[aresta]][v_ini] = 0; 
            vertices--; 
            v_ini = arestas_vertice[aresta];

            // break from inner and outer loops
            aresta = n_arestas_vertice;
            
        }
        
       
    }


    if(passos == size){
        printf("é euleriano\n");    
    }else{
        printf("nao é euleriano\n");
    }
    for(int i = 0; i <= passos; i++){
        printf("%d - ", caminho[i]);
    }

} 
/*------------------------------------------------------------------------------------------------*/



int main(){

/*
0, 1, 0, 1, 1, 1, 
1, 0, 1, 0, 1, 0, 
0, 1, 0, 1, 1, 0, 
1, 0, 1, 0, 1, 1, 
1, 1, 1, 1, 0, 0, 
1, 0, 0, 1, 0, 0,
*/ 

    grafo[0][0] = 0;
    grafo[0][1] = 1;
    grafo[0][2] = 0;
    grafo[0][3] = 1;
    grafo[0][4] = 1;
    grafo[0][5] = 1;

    grafo[1][0] = 1;
    grafo[1][1] = 0;
    grafo[1][2] = 1;
    grafo[1][3] = 0;
    grafo[1][4] = 1;
    grafo[1][5] = 0;

    grafo[2][0] = 0;
    grafo[2][1] = 1;
    grafo[2][2] = 0;
    grafo[2][3] = 1;
    grafo[2][4] = 1;
    grafo[2][5] = 0;

    grafo[3][0] = 1;
    grafo[3][1] = 0;
    grafo[3][2] = 1;
    grafo[3][3] = 0;
    grafo[3][4] = 1;
    grafo[3][5] = 1;

    grafo[4][0] = 1;
    grafo[4][1] = 1;
    grafo[4][2] = 1;
    grafo[4][3] = 1;
    grafo[4][4] = 0;
    grafo[4][5] = 0;

    grafo[5][0] = 1;
    grafo[5][1] = 0;
    grafo[5][2] = 0;
    grafo[5][3] = 1;
    grafo[5][4] = 0;
    grafo[5][5] = 0;



    int start = buscarInicial(NODE, grafo); 

    printf("start: %d\n", start);
    fleury(NODE, grafo, start);

    return 0;
}