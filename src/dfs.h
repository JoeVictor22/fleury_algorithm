#include <stdbool.h>
#include <stdio.h>
#include "grafo.h"

/*------------------------------------------------------------------------------------------------*/
  
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/*
Para implementar a DFS de forma iterativa, usamos um vetor de inteiros que ira ser interpretado como 
uma pilha. O estado vazio de um posicao da pilha é indicado por -1, e as funcoes para operar sobre
a pilha estão implementadas a seguir
*/
/*                PILHA                */
void iniciarVetor(int *vector, int size){
    for(int i=0; i < size;i++){
        vector[i] = -1;
    }
}

//*** TODO mudar o nome das funcoes e variaveis ***//
// Verifica se a pilha esta preenchida com somente valores -1
// Caso verdade a pilha se encontra vazia retorna TRUE
bool ehVazio(int *pilha, int size){
    for(int i=0; i < size; i++){
        if(pilha[i] != -1){
            return false;
        }    
    }
    return true;
    
}

void empurrar(int *pilha, int size, int value){
    int i;
    for(i = 0; i < size; i++){
        if(pilha[i] == -1){
            break;
        }
    }
    pilha[i] = value;
}

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
/*                END                */

/*
Funcoes auxiliares usadas para DEBUG do codigo
*/

/*------------------------------------------------------------------------------------------------*/
/*
Implementacao iterativa do DFS modificada para retornar a quantidade de arestas acessiveis dado um 
vertice inicial
*/
int dfs(int size, int (*grafo)[size], int inicial){
    //Pilha de proximos a entrar na DFS
    int pilha[size];
    iniciarVetor(pilha, size);
    
    bool visitado[size];
    for(int i = 0; i < size; i++){
        visitado[i] = false;
    }

	//A primeira vertice na DFS é o inicial passado como argumento
    empurrar(pilha, size, inicial);
    visitado[inicial] = true;
	
	//Realiza verificações até a pilha estar vazia
    while(!ehVazio(pilha, size)){ 
        
        int vertice;
		//Vertice é a vertice onde ira ser feito a DFS
        vertice = sacar(pilha, size);
				
        //Recebe um vetor que é a linha do Vertice a ser feita a DFS
        int vetorVizinhos[size];
        copiarGrafoLinha(size, vetorVizinhos, grafo, vertice);

        //printLinha(size, vetorVizinhos);
        
		// "i" itera sobre o vetor de Vertices e marca os vizinhos ao Vertice utilizado
        for(int i=0; i < size; i++){
			//Caso o vertice seja vizinho, (==1), e ainda não tenha sido marcado como visitado
			//O vizinho será colocado na pilha para verificar os seus proprios vizinhos e marcado como visitado
            if(vetorVizinhos[i] == 1 && !visitado[i]){
                empurrar(pilha, size, i);
                visitado[i] = true;
            }
        }
    }

	//Realiza uma contagem de quantos vertices é possivel atingir a partir do inicial
    int count = 0;
    for(int i = 0; i < size; i++){
        if(visitado[i] == true){
            count++;
        }
    }

	//Retornar a contagem para comparações
    return count;
}

