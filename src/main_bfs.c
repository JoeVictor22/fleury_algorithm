
#include <stdbool.h>
#include <stdio.h>
#define NODE 6
/*------------------------------------------------------------------------------------------------*/
int grafo[NODE][NODE];
  
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int qtdArestas(){
    int arestas=0;
    for (int i=0;i<NODE;i++){
        for (int j=i;j<NODE;j++){
            arestas+=grafo[i][j];
        }
    }
    printf("n_aresta=%d\n",arestas);
    return arestas;
}
/*------------------------------------------------------------------------------------------------*/
//int *start_vector(int *vector, int size){
void start_vector(int *vector, int size){

    printf("sv\n");
    //int vector[size];
    for(int i=0; i < size;i++){
        vector[i] = -1;
    }
    //return vector;
}

// o indicador se a posicao esta vazia é -1
bool empty(int *fila, int size){
    printf("emp\n");
    for(int i=0; i < size; i++){
        if(fila[i] != -1){
            return false;
        }    
    }
    printf("seg\n");
    return true;
    
}
int begin(int *fila){
    printf("b\n");
    return fila[0];
}

void erase(int *fila, int size, int remover){
    printf("eras\n");
    fila[remover] = -1;
    int i;
    for(i = 0; i < size; i++){
        if(fila[i] == -1){
            break;
        }
    }
    for(i; i < size; i++){
        fila[i] = fila[i+1];   
    }
    fila[size-1] = -1;
}
void push_back(int *fila, int size, int value){
    printf("pb\n");
    int i;
    for(i = 0; i < size; i++){
        if(fila[i] == -1){
            break;
        }
    }
    fila[i] = value;

}


/*------------------------------------------------------------------------------------------------*/
bool bfs(int start, int end){
    printf("bfs\n");
    bool visited[NODE];

    for(int i = 0; i < NODE; i++){
        visited[i] = false;
    }

    int q[NODE];
    start_vector(q, NODE);
    push_back(q, NODE, start);
    printf("\ngrafo\n");
    for(int i =0 ; i < NODE; i++){
        printf(" %d ,", q[i]);
    }
    char c;
    //scanf("%c", &c);
    visited[start] = true;
    int vis;
    int v_ini;
    while(!empty(q, NODE)){
        vis = q[0];
        if(vis == -1){
            vis = 0;
        }


        erase(q, NODE, 0);
        printf("vis: %d\n", vis);
        

        //scanf("%c", &c);
        for (int i = 0; i < NODE; i++){
            printf("%d %c\n", grafo[vis][i], (visited[i] ? 'S': 'N'));
            if(grafo[vis][i] == 1 && (!visited[i])){
                push_back(q, NODE, i);
                visited[i] = true;
            }
        }

   
    }

    if (visited[end]){
        return true;
    }

    return false;

}

/*------------------------------------------------------------------------------------------------*/


int main(){
    printf("comeca\n");

    //int n_arestas = qtdArestas();
    
    // start ---> end
    // prestar atencoa no indice 0 ou 1
   

    /*
    {0, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 0, 0},
    {1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 0},
    {1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0}
    };
    */
    grafo[0][0] = 0;
    grafo[0][1] = 1;
    grafo[0][2] = 1;
    grafo[0][3] = 0;
    grafo[0][4] = 1;
    grafo[0][5] = 1;
    
    grafo[1][0] = 1;
    grafo[1][1] = 0;
    grafo[1][2] = 1;
    grafo[1][3] = 1;
    grafo[1][4] = 0;
    grafo[1][5] = 0;
    
    grafo[2][0] = 1;
    grafo[2][1] = 1;
    grafo[2][2] = 0;
    grafo[2][3] = 1;
    grafo[2][4] = 1;
    grafo[2][5] = 0;

    grafo[3][0] = 0;
    grafo[3][1] = 1;
    grafo[3][2] = 1;
    grafo[3][3] = 0;
    grafo[3][4] = 1;
    grafo[3][5] = 0;
    
    grafo[4][0] = 1;
    grafo[4][1] = 0;
    grafo[4][2] = 1;
    grafo[4][3] = 1;
    grafo[4][4] = 0;
    grafo[4][5] = 1;
    
    grafo[5][0] = 1;
    grafo[5][1] = 0;
    grafo[5][2] = 0;
    grafo[5][3] = 0;
    grafo[5][4] = 1;
    grafo[5][5] = 0;
    printf("comeca\n");

    int start = 1;
    int end = 3;
    if (bfs(start, end)){
        printf("chega\n");
    }else{
        printf("nao chega\n");
    }

    return 0;
}