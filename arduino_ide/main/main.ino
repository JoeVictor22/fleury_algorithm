
#define button2 2
#define button3 3

// max size para o numero de vertices
#define MAX_SIZE 22
// min size para o numero de vertices
#define MIN_SIZE 2
// max size para o caminho percorrido no pior caso possivel: (37 * 36)/2
#define MAX_CAMINHO 245
// no pior caso, o algoritmo deve usar cerca de 2069 inteiros

// matriz utilizada para a computação dos dados
int grafo_matriz[MAX_SIZE][MAX_SIZE];
int caminho[MAX_CAMINHO];


void setup() {
  Serial.begin(9600);
  Serial.println("\nSTART: ");

  delay(100);
}
void loop() {

  int opcao = 0;
  opcao = leitura();

  switch(opcao){
    case 2:
      Serial.println("OPC 1");
      fleury_algoritm();
      break;
    case 3:
      Serial.println("OPC 2");
      break;
    default:
      Serial.println("Roda teste");
      //fleury_algoritm();
      break;
      
  }
  delay(100);
}

/*
Leitura dos pinos de entrada
*/
int leitura() {
  if (digitalRead(button2) == HIGH) {
    return 1;
  } if (digitalRead(button3) == HIGH) {
    return 2;
  } else {
    return 0;
  }
}

void fleury_algoritm(){
  
  // Teste do código
  teste();
}


// Função para teste exaustivo da aplicação
void teste(){
  Serial.println("\n\nTESTES"); 
  int i;
  for( i = MIN_SIZE; i <= MAX_SIZE; i++){
    Serial.print("\nSIZE: ");
    Serial.println(i);

    fillMatrix(i, 0);

    create(i);
    // Busca um vertice para iniciar a verificação do grafo
    int start = buscarInicial(i); 

    // Trabalha para encontrar um caminho Euleriano no grafo
    fleury(i, start);
  } 
}


//Preenche dada matriz com um valor recebido
void fillMatrix( int size, int value){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      grafo_matriz[i][j] = value;
    }
  }
}

/*
Utilitario para gerar uma matriz, caso o tamanho seja impar ela sera euleriana
*/
void create(int size){
  for(int i =0 ; i < size; i++){
    for( int j=0; j < size; j++){
      if (j != i){
        grafo_matriz[i][j] = 1;
      }
    }
  }
}


// Busca o vertice mais indicado para dar inicio no caminho
// Busca um vertice com grau ímpar, caso não exista nenhum, retorna um vertice qualquer do grafo
int buscarInicial(int numVert){
  int grau = 0;
  for(int i=0; i < numVert; i++){
    grau = 0;
    for(int j = 0; j < numVert; j++){
      if (grafo_matriz[i][j] == 1){
        grau++;
      }
    }
    if((grau % 2 ) == 1 ){
      return i;
    }
  } 
  return 0;
}


// Busca um caminho ou ciclo euleriano no grafo assim como avalia algumas caracteristicas do grafo
void fleury(int size, int start){
  // Contagem de vertices visitados
  int vertices_visitados = 0;
  int v = start;
  
  /* Calcula o tamanho maximo do caminho que pode ser percorrido
  int max_caminho_size = ((size * (size-1))/2);
  if(max_caminho_size > MAX_CAMINHO){
    max_caminho_size = MAX_CAMINHO;
  }

  // Caminho feito no grafo a ser impresso
  int caminho[max_caminho_size];
  */
  caminho[vertices_visitados] = start; 
  vertices_visitados++;
  
  while(true){
    // Cria um vetor com as arestas dado o vertice inicial
    int arestas_vertice[size];
	
    // Quantidade de arestas naquele vertice
    int n_arestas_vertice = 0;
    n_arestas_vertice = countArestas(size, arestas_vertice, start);

    // Itera sobre as arestas
    int i;
    for (i = 0; i < n_arestas_vertice; i++){
    // V recebe a Aresta 
	  v = arestas_vertice[i];
	  
      // Verfica se a posição no Grafo é valida e se a Aresta não é de corte
      if (grafo_matriz[start][v] == 1 && ehArestaCorte(start, v, arestas_vertice, n_arestas_vertice, size)){
	    	// Caso a Aresta seja validada ela é removida do Grafo
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0;
		
	    	// O ponto final da Aresta é se torna o ponto inicial para as proximas verificações
        start = v;  

	    	// Guarda o Vertice para printar caminho
        caminho[vertices_visitados] = start; 
        vertices_visitados++;
        break;
      }
    }
	
	  // Caso o Vertice não seja validado anteriormente é escolha a ultima Aresta
    if(i >= n_arestas_vertice){
        // Remove a Aresta do Grafo
        grafo_matriz[start][v] = 0;
        grafo_matriz[v][start] = 0; 
		
        // O ponto final da Aresta é se torna o ponto inicial para as proximas verificações
        start = v;
		
        // Guarda o Vertice para printar caminho
        caminho[vertices_visitados] = start; 
        vertices_visitados++;
    }

  	// Caso de saida uma Vertice que não possui Arestas
    if(n_arestas_vertice == 0){
	    // Remove pois visita a si mesmo garantindo ser o ultimo ponto
	    vertices_visitados--; 
      break;
    }
  }

  // Printa o Grafo após percorer todos os caminhos validos
  printGrafo(size);

  // Um Grafo contem um caminho Euleriano se somente se ele percore todas as arestas contidas nele.
  if (qtdArestas(size) == 0){
    Serial.print("\nCAMINHO ");
    for( int i = 0 ; i < vertices_visitados; i++){
      Serial.print("-> ");
      Serial.print(caminho[i]);
    }
  }else{
    Serial.println("\nO grafo não possui trilha euleriana!");
  }

  Serial.println(" ");
} 

// Retorna a quantidade de arestas de um vertice e salva essas vertices em um vetor 
int countArestas(int size, int *arestas, int row){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(grafo_matriz[row][i] == 1){
            arestas[count] = i;
            count++;
        }
    }
    return count;
}



// Valida a Aresta do grafo a ser utilizada
bool ehArestaCorte(int start, int end, int *adjacentes, int n_arestas_vertice,
                   int size){
  // Contabiliza a quantidade de ligações que o Vertice possui
  int count = 0;
  for (int i = 0; i < n_arestas_vertice; i++){ 
    if(grafo_matriz[start][i] == 1){
      count++;
    }
  }
  
  // Caso o Vertice só possua uma Aresta é realizado uma DFS a fim de reconhecer se ele é uma "Aresta de corte"
  if (count == 1){
  	// Salva a quantidade de Vertices acessiveis com a Aresta presente no grafo
    int dfs_c_aresta = dfs(size, end);
    
    grafo_matriz[end][start] = 0;
    grafo_matriz[start][end] = 0;

  	// Salva a quantidade de Vertices acessiveis sem a Aresta presente no grafo
    int dfs_s_aresta = dfs(size, end);

    grafo_matriz[end][start] = 1;
    grafo_matriz[start][end] = 1;

  	// Ao comparar o resultado retornado pelas DFS's é definido se a aresta será usada.
    return (dfs_c_aresta > dfs_s_aresta) ? false : true;
  
  /* Caso não seja possivel determinar via uma dfs, essa aresta sera considerada de corte se e somente se a quantidade de
   vertices conectados seja igual a 0
  */
  }else if(count > 1){
    return false;
  }else{
    return true;
  }
}

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

// Copia somente uma linha do Grafo para um vetor
void copiarGrafoLinha(int size, int *copy, int row){   
    for(int j = 0; j < size; j++){
        copy[j] = grafo_matriz[row][j];
    }
}
/*--------------------------------------------------------------------------------------------------------*/

// Printa o Grafo em sua integridade
void printGrafo(int size){
    Serial.print("\nGRAFO:\n");
    for( int i =0 ; i < size; i++){
        for( int j= 0; j < size; j++){
            Serial.print(grafo_matriz[i][j]);
            Serial.print(" ");
        }
        Serial.println(" ");
    }
}

// Calcula a quantidade de arestas presentes no grafo
int qtdArestas(int size){
  int arestas=0;
  for (int i=0;i<size;i++){
    for (int j=i;j<size;j++){
      arestas+=grafo_matriz[i][j];
    }
  }
  return arestas;
}
