def deep(l):
	  dupl=[]
	  for i in range(len(l)):
	    dupl.append([])
	    for j in range(len(l[0])):
	      dupl[i].append(l[i][j])
	  return dupl
	  
def det(matrix):
  if not len(matrix[0])==len(matrix):
    raise Exception("Cannot find determinant of rectangular matrix.")
	
  def cofactor(matrix1,i,j):
    mat=deep(matrix1)
    mat.pop(i)
    for k in range(len(mat)):
      mat[k].pop(j)
    return mat
    
  if len(matrix)==2 and len(matrix[0])==2:
    return (matrix[0][0]*matrix[1][1])-(matrix[0][1]*matrix[1][0])   
  
  else:
    n=len(matrix)
    i=0
    sum=0
    for j in range(n):
      sum+=((-1)**(i+j))*(matrix[i][j])*(det(cofactor(matrix,i,j)))
  return sum
  
'''row=int(input("Enter the no. of row: "))
col=int(input("Enter the no. of columns: "))
a=[]
for i in range(row):
  a.append([])
  for j in range(col):
    a[i].append(int(input('Enter the no.: ')))
print(det(a))'''