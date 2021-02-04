import serial
from pprint import pprint

def create(size):
    grafo = [[0 for i in range(size)] for j in range(size)] 
    for i in range(size):
        for j in range(size):
            if j != i:
                grafo[i][j] = 1
    return grafo

def validar(grafo, caminho):
    for i in range(len(caminho) - 1):
        if grafo[caminho[i]][caminho[i+1]] == 1:
            print("->" + caminho[i], end=" ")
            grafo[caminho[i]][caminho[i+1]] = 0
        else:        
            return False
    return True



arduino = serial.Serial('COM7', 9600)

size = 10
grafo = [[0 for i in range(size)] for j in range(size)] 

grafo = create(size)

arduino.write(B'5')

pprint(grafo)

caminho = arduino.read()

#if not validar(grafo, caminho, size):
#    print("ERROR na validação")

