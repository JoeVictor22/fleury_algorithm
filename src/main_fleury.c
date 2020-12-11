
#include <stdbool.h>
#include <stdio.h>

/*------------------------------------------------------------------------------------------------*/
int buscarInicial(int numVert, int grafo[numVert][numVert]){


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
bool ehPonte(int v, int numVert, int grafo[numVert][numVert]){
  int degrau = 0;
  for (int i = 0; i < numVert; i++){
    if(grafo[v][i] == 1){
          degrau++;
      }
    if (degrau > 1){
        return false;
    }
  }
  return true;
}


/*------------------------------------------------------------------------------------------------*/
int qtdArestas(int numVert, int grafo[numVert][numVert]){
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

void fleury(int v_inicio, int caminho[], int numVert, int grafo[numVert][numVert], int arestas){
  int aresta = v_inicio;
  bool flag = true;
  int countCam = 0;
  while(flag)
    flag = false;
    for (int i = 0; i < numVert; i++){


      
      if(grafo[aresta][i] == 1 && (arestas <= 1 || ehPonte(i,numVert,grafo)) ){
        printf("\naresta: %i - %i - %s\n", aresta, i, (ehPonte(i,numVert,grafo)? "S": "N"));
        caminho[countCam]=aresta;
        caminho[countCam+1]=i;

        grafo[aresta][i] = 0;
        arestas--; 
        aresta = i;
        flag = true;
        countCam+=2;
      }
  }
  printf("arestas no fim:%d", arestas);
  if(!arestas){
    printf("\n");
    for (int i=0;i<countCam;i+=2){
      printf("%i->%i // ", caminho[i], caminho[i+1]);
    }
    printf("\n");
 }
  else{
    printf("\ngrafo not gudo"); //GRAFO NAO EULERIANO!
  }
  
} 
/*------------------------------------------------------------------------------------------------*/


int main(){
  int numVert;
  scanf("%d",&numVert);
  int grafo[numVert][numVert];

  for(int i=0;i < numVert;i++){
    for(int j=0; j < numVert;j++){
      scanf("%d", &grafo[i][j]);
    }
  }

  int arestas=qtdArestas(numVert, grafo);
  int caminho[arestas*2];
  int ini = buscarInicial(numVert, grafo);
  printf("ini = %i\n", ini);
  fleury(ini, caminho, numVert, grafo, arestas);
  return 0;
}