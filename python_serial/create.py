from pprint import pprint

def create(size):
    grafo = [[0 for i in range(size)] for j in range(size)] 
    for i in range(size):
        for j in range(size):
            if j != i:
                grafo[i][j] = 1
    return grafo



tamanho = input("Qual o tamanho da matriz a ser criada?\n")
tamanho = int(tamanho)
grafo = create(tamanho)

for i in range(tamanho):
    for j in range(tamanho):
        print(str(grafo[i][j]) + " ", end='')
    print("")