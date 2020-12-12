
#include <stdbool.h>
#include <stdio.h>
/*------------------------------------------------------------------------------------------------*/
  
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
//int *start_vector(int *vector, int size){
void start_vector(int *vector, int size){

    //int vector[size];
    for(int i=0; i < size;i++){
        vector[i] = -1;
    }
    //return vector;
}

// o indicador se a posicao esta vazia é -1
bool empty(int *fila, int size){
    for(int i=0; i < size; i++){
        if(fila[i] != -1){
            return false;
        }    
    }
    return true;
    
}
int begin(int *fila){
    return fila[0];
}

void erase(int *fila, int size, int remover){
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
    int i;
    for(i = 0; i < size; i++){
        if(fila[i] == -1){
            break;
        }
    }
    fila[i] = value;

}


/*------------------------------------------------------------------------------------------------*/
int bfs(int size, int (*grafo)[size], int start){
    bool visited[size];

    for(int i = 0; i < size; i++){
        visited[i] = false;
    }

    int q[size];
    start_vector(q, size);
    push_back(q, size, start);
    
    char c;
    visited[start] = true;
    int vis;
    while(!empty(q, size)){
        vis = q[0];

        erase(q, size, 0);      

        for (int i = 0; i < size; i++){
            if(grafo[vis][i] == 1 && (!visited[i])){
                push_back(q, size, i);
                visited[i] = true;
            }
        }

   
    }

    int count = 0;
    printf("grafos visitados a partir de %d\n", start);
    for(int i = 0; i < size; i++){
        printf("%c, ", (visited[i]? 'S': 'N'));
        if(visited[i] == true){
            count++;
        }
    }
    printf("\n\n");

    return count;

}
