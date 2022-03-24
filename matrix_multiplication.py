def mult(a,b):
  if not (len(a[0])==len(b)):
    print("Matrix multiplication not compatible.")
  else:
    product=[]
    m=len(a)
    p=len(b[0])
    n=len(b)
    for i in range(m):
      product.append([])
      for j in range(p):
        sum=0
        for k in range(n):
          sum+=a[i][k]*b[k][j]
        product[i].append(sum)
    return product
row1=int(input("Enter the no. of row"))
col1=int(input("Enter the no. of columns"))
a=[]
for i in range(row1):
  a.append([])
  for j in range(col1):
    a[i].append(int(input('Enter the no.: ')))

row2=int(input("Enter the no. of row"))
col2=int(input("Enter the no. of columns"))
b=[]
for i in range(row2):
  b.append([])
  for j in range(col2):
    b[i].append(int(input('Enter the no.: ')))

print(mult(a,b))