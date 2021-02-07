import time
from pprint import pprint

def validar(grafo, caminho):
    for i in range(len(caminho) - 1):
        if grafo[caminho[i]][caminho[i+1]] == 1:
            #print("->" + strcaminho[i], end=" ")
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
    size = [int(x) for x in next(f).split()] # read first line
    
    for line in f: # read rest of lines
        grafo.append([int(x) for x in line.split()])


with open('caminho') as f:
    [caminho.append(int(x)) for x in next(f).split()] # read first line
    
pprint(grafo)
pprint(caminho)

if validar(grafo, caminho):
    print("Trilha valida")
else:
    print("Trilha invalida")

#pprint(grafo)