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
def fromMatrixToVector(row, col, N):
   if (row<col):
      return int(row*(N-1) - (row-1)*(row)/2 + col - row - 1)
   elif (col<row):
      return int(col*(N-1) - (col-1)*(col)/2 + row - col - 1)
   else:
      return -1

def create(size):
    grafo = [[0 for i in range(size)] for j in range(size)] 
    for i in range(size):
        for j in range(size):
            if j != i:
                grafo[i][j] = 1
    return grafo

length = 5


matriz = create(int(length))


index = 0
for i in range(0, length):    
    for j in range(0, length):
        pos = fromMatrixToVector(i, j, length)

        print(str(i) + " - " +str(j) + " : " +  str(pos))
        index+=1





