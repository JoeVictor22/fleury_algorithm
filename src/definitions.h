// max size para o numero de vertices
#define MAX_SIZE 37
// min size para o numero de vertices
#define MIN_SIZE 2
// max size para o caminho percorrido no pior caso possivel: (37 * 36)/2
#define MAX_CAMINHO 700
// no pior caso, o algoritmo deve usar cerca de 2069 inteiros

// matriz utilizada para a computação dos dados
int grafo_matriz[MAX_SIZE][MAX_SIZE];
int matriz_copia_teste[MAX_SIZE];
