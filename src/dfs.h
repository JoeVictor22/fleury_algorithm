
#include <stdbool.h>
#include <stdio.h>
/*------------------------------------------------------------------------------------------------*/
  
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/*
Para implementar a DFS de forma iterativa, usamos um vetor de inteiros que ira ser interpretado como 
uma pilha. O estado vazio de um posicao da pilha é indicado por -1, e as funcoes para operar sobre
a pilha estão implementadas a seguir
*/
/*                PILHA                */
void start_vector(int *vector, int size){
    for(int i=0; i < size;i++){
        vector[i] = -1;
    }
}

// TODO mudar o nome das funcoes e variaveis
// o indicador se a posicao esta vazia é -1
bool empty(int *pilha, int size){
    for(int i=0; i < size; i++){
        if(pilha[i] != -1){
            return false;
        }    
    }
    return true;
    
}
int begin(int *pilha){
    return pilha[0];
}

void erase(int *pilha, int size, int remover){
    pilha[remover] = -1;
    int i;
    for(i = 0; i < size; i++){
        if(pilha[i] == -1){
            break;
        }
    }
    for(i; i < size; i++){
        pilha[i] = pilha[i+1];   
    }
    pilha[size-1] = -1;
}
void push_back(int *pilha, int size, int value){
    int i;
    for(i = 0; i < size; i++){
        if(pilha[i] == -1){
            break;
        }
    }
    pilha[i] = value;
}

int pop(int *pilha,int size){
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
/*                END                */

/*
Funcoes auxiliares usadas para DEBUG do codigo
*/
void copy_graph_row(int size, int *copy, int (*orig)[size], int row){   
    for(int j = 0; j < size; j++){
        copy[j] = orig[row][j];
    }
}
void print_graph(int size, int (*grafo)[size]){
    printf("\nGRAFO:\n");
    for( int i =0 ; i < size; i++){
        for( int j= 0; j < size; j++){
            printf("%d ", grafo[i][j]);
        }
        printf("\n");
    }
}
void print_row(int size, int *vector){
    printf("\nLINHA:\n");
    for(int i = 0; i < size; i++){
        printf("%d ", vector[i]);
    }
    printf("\n");
}


/*------------------------------------------------------------------------------------------------*/
/*
Implementacao iterativa do DFS modificada para retornar a quantidade de arestas acessiveis dado um 
vertice inicial
*/
int dfs(int size, int (*grafo)[size], int inicial){
    //Pilha de proximos a entrar na DFS
    int pilha[size];
    start_vector(pilha, size);
    
    bool visitado[size];
    for(int i = 0; i < size; i++){
        visitado[i] = false;
    }

    push_back(pilha, size, inicial);      //Primeiro a entrar na DFS é o inicial passado como argumento WHATEVER!

    visitado[inicial] = true;  //CLARO Q ELE EH TRUE DUH!

    while(!empty(pilha, size)){ //RECURSAUM SO QUE NAO!
        //print_graph(size, grafo);
        int aresta;
        aresta = pop(pilha, size); //aresta é o no onde ira ser feito a DFS
        //printf("\naresta: %d, ", aresta);
        //recebe um vetor que eh a linha do nó a ser feita a DFS
        int vetorVizinhos[size];
        copy_graph_row(size, vetorVizinhos, grafo, aresta);

        //print_row(size, vetorVizinhos);
        
        for(int i=0; i < size; i++){// "i" itera sobre o vetor e marca aonde é possivel chegar
            if(vetorVizinhos[i] == 1 && !visitado[i]){
                push_back(pilha, size, i);
                visitado[i] = true;
            }
        }
        //char wait;
        //scanf("%c", &wait);
    }

    int count = 0;
    //printf("\ngrafos visitados a partir de %d\n", inicial);
    for(int i = 0; i < size; i++){
        //printf("%c, ", (visitado[i]? 'S': 'N'));
        if(visitado[i] == true){
            count++;
        }
    }
    //printf("\n\n");
    //char wait;
    //scanf("%c", &wait);

    return count;
}

