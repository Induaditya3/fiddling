from determinant import *

#solutions of systems of simultaneous equations using cramer's rule

x=int(input("Enter the no. of unknowns:"))
coefficient=[]
constants=[]
for i in range(x):
  l=[]
  for j in range(x+1):
    if j==x:
      b=int(input(f"Enter the constant b{i}: "))
      constants.append(b)
    else:
      a=int(input(f"Enter the coefficient a{i}{j}: "))
      l.append(a)
  coefficient.append(l)
delta=det(coefficient)
c=1
while True:
  coefficient1=deep(coefficient)
  for i in range(x):
    coefficient1[c-1][i]=constants[i]
  
  temp=det(coefficient1)
  print(f"x{c-1}={temp/delta}")
  c+=1
  if c==x+1:
    break 