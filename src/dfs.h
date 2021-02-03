#include <stdbool.h>
#include <stdio.h>
#include "definitions.h"

/*------------------------------------------------HEADERS-------------------------------------------------*/
// Import explicito de funcoes do grafo.h para evitar erros 
void copiarGrafoLinha(int size, int *copy, int row);   
void printGrafo(int size);
void printLinha(int size, int *vector);
void addAresta( int size, int i, int j);
bool ehArestaCorte(int start, int end, int *adjacentes, int n_arestas_vertice, int size);
int qtdArestas(int size);
int countArestas(int size, int *arestas, int row);
void fillMatrix( int size, int value);
/*--------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------*/
/*Para implementar a DFS de forma iterativa, foi utilizado um vetor de inteiros que é interpretado como   */
/*uma pilha. O estado vazio de um posição da pilha é indicado por -1, e as funções para operar sobre      */
/*a pilha estão implementadas a seguir.                                                                   */
/*--------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------PILHA-------------------------------------------------------*/
void iniciarVetor(int *vector, int size){
    for(int i=0; i < size;i++){
        vector[i] = -1;
    }
}

// Verifica se a pilha esta preenchida com somente valores -1
// Caso verdade a pilha se encontra vazia, retorna TRUE.
bool ehVazio(int *pilha, int size){
    for(int i=0; i < size; i++){
        if(pilha[i] != -1){
            return false;
        }    
    }
    return true;
}

// Insere um valor no topo da pilha
void empurrar(int *pilha, int size, int value){
    int i;
    for(i = 0; i < size; i++){
        if(pilha[i] == -1){
            break;
        }
    }
    pilha[i] = value;
}

// Saca o valor que se encontra no topo da pilha
int sacar(int *pilha,int size){
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
/*--------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------DFS-------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------*/
/*Implementação iterativa do DFS modificada para retornar a quantidade de arestas acessiveis dado um      */
/*vertice inicial                                                                                         */
/*--------------------------------------------------------------------------------------------------------*/
int dfs(int size, int inicial){
    // Pilha de proximos a entrar na DFS
    int pilha[size];
    iniciarVetor(pilha, size);
    
	// Vetor boolenao armazenando vertices que são possiveis ser acessados
    bool visitado[size];
    for(int i = 0; i < size; i++){
        visitado[i] = false;
    }

	// A primeira vertice na DFS é o inicial passado como argumento
    empurrar(pilha, size, inicial);
    visitado[inicial] = true;
	
	// Realiza verificações até a pilha estar vazia
    while(!ehVazio(pilha, size)){ 
        
		// Vertice é a vertice onde ira ser feito a DFS
        int vertice;
        vertice = sacar(pilha, size);
				
        // Recebe um vetor que é a linha da Matriz correspondente ao Vertice a ser feita a DFS
        int vetorVizinhos[size];
        copiarGrafoLinha(size, vetorVizinhos, vertice);
        
		// "i" itera sobre o vetor e marca os vizinhos ao Vertice utilizado
        for(int i=0; i < size; i++){
			// Caso o vertice seja vizinho, (==1), e ainda não tenha sido marcado como visitado
			// O vizinho será colocado na pilha para verificar os seus proprios vizinhos e marcado como visitado
            if(vetorVizinhos[i] == 1 && !visitado[i]){
                empurrar(pilha, size, i);
                visitado[i] = true;
            }
        }
    }

	// Realiza uma contagem de quantos vertices é possivel atingir a partir do inicial
    int count = 0;
    for(int i = 0; i < size; i++){
        if(visitado[i] == true){
            count++;
        }
    }

	// Retornar a contagem para comparações
    return count;
}
/*--------------------------------------------------------------------------------------------------------*/
