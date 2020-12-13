#include <stdbool.h>
#include <stdio.h>

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
void addAresta( int size, int i, int j, int (*matriz)[size]){
  matriz[i][j] = 1;
  matriz[j][i] = 1;
}