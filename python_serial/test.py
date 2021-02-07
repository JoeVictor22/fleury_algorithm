import serial
import time
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

def ler_arduino():
    print("Aguardando resposta: ")    
    resposta = arduino.readline()
    print(resposta.decode())



arduino = serial.Serial('COM7', 9600, timeout=2)

#grafo = create(size)
#pprint(grafo)



while(True):
    
    ler_arduino()

    tamanho = input("Digite o tamanho da matriz\n")
    if tamanho != "":
        tamanho = str(tamanho)
        #arduino.write(str(chr(int(tamanho))).encode())
        arduino.write(tamanho.encode())

    ler_arduino()
    for i in range(int(tamanho)):
        for j in range(int(tamanho)):
            entrada = input()
            
            if entrada != "":
                entrada = int(entrada)
                #arduino.write(str(chr(entrada)).encode())
                arduino.write(entrada.encode())
            else:
                print("valor invalido, " + str(entrada))
                j-=1


    print("Aguardando caminho: ")    
    caminho = arduino.readline()
    
    print(caminho.decode())

    while arduino.readable():
        caminho = arduino.readline()
        if str(caminho) == "A":
            print('fim do caminho')
            break; 
        print(caminho.decode())
        


    
#if not validar(grafo, caminho, size):
#    print("ERROR na validação")


