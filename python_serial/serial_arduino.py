from serial import Serial
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



arduino = Serial('COM7', 9600, timeout=4)

#grafo = create(size)
#pprint(grafo)

print("lendo")
tamanho = 0

while True:
    if(arduino.readable()):
        print("\nPYTHON: " + str(tamanho))
        arduino.write(str(chr(int(tamanho))).encode())
        caminho = arduino.read()
        print("ARDUINO: " + str(caminho.decode()), end='')
        if tamanho >= 250: tamanho = 0
        tamanho = tamanho + 1
    time.sleep(0.1)

while(True):
    print("Digite o tamanho:")
    tamanho = input()
    if tamanho != "":
        arduino.write(str(chr(int(tamanho))).encode())
        #arduino.write(str(tamanho).encode())
    
    ler_arduino()
    print("Digite os valores:")
    for i in range(int(tamanho)):
        for j in range(int(tamanho)):
            entrada = input()
            
            if entrada != "":
                arduino.write(str(chr(int(entrada))).encode())
                #arduino.write(str(entrada).encode())
            else:
                print("valor invalido, " + str(entrada))
                j-=1


    print("Aguardando caminho: ")    
    ler_arduino()

    while arduino.readable():
        caminho = arduino.readline()
        print(caminho.decode())
        


    
#if not validar(grafo, caminho, size):
#    print("ERROR na validação")