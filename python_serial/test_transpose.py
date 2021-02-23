'''
matriz = [
    [0,1,1,1,1,1],
    [1,0,1,1,1,1],
    [1,1,0,1,1,1],
    [1,1,1,0,1,1],
    [1,1,1,1,0,1],
    [1,1,1,1,1,0]
    ]
'''
'''
matriz = [
    [0,1,1,1,1],
    [1,0,1,1,1],
    [1,1,0,1,1],
    [1,1,1,0,1],
    [1,1,1,1,0]
    ]
'''

def create(size):
    grafo = [[0 for i in range(size)] for j in range(size)] 
    for i in range(size):
        for j in range(size):
            if j != i:
                grafo[i][j] = 1
    return grafo

length = 100

matriz = create(length)


def fromMatrixToVector(i, j,  N):
   if i <= j:
      return int(i * N - (i - 1) * i / 2 + j - i - N)
   else:
      return int(j * N - (j - 1) * j / 2 + i - j - N)

index = 0
for i in range(1, length):
    
    for j in range(i, length):
        pos = fromMatrixToVector(i, j, length)
        if index != pos:
            print("posicao errado")
            exit()

        print(str(i) + " - " +str(j+1) + " : " + str(index) + " -- " + str(pos))
        index+=1





