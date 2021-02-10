import time
from pprint import pprint

def validar(grafo, caminho):
    for i in range(len(caminho) - 1):
        print(caminho[i] ,end=" -> ")
        print(caminho[i+1])

        if grafo[caminho[i]][caminho[i+1]] == 1:
            grafo[caminho[i]][caminho[i+1]] = 0
            grafo[caminho[i+1]][caminho[i]] = 0
        else:        
            return False
    
    for i in range(len(grafo)):
        for j in range(len(grafo)):
            if grafo[i][j] == 1:
                return False

    return True


grafo = []
caminho = []
with open('grafo') as f:
    size = [int(x) for x in next(f).split()]
    
    for line in f:
        grafo.append([int(x) for x in line.split()])


with open('caminho') as f:
    #caminho.append( [int(x) for x in next(f).split()])
    for line in f:
        for i in line.split():
            caminho.append(int(i))
    
for i in grafo:
    print(i)


if validar(grafo, caminho):
    print("Trilha valida")
else:
    print("Trilha invalida")

for i in grafo:
    print(i)
