change={25:'quarter',10:'ten',5:'five',1:'cent'}
count={i:0 for i in change}
order=[1,5,10,25]
while True:
  money=input("Change owed: ")
  if money.isdigit():
    money=int(money)
    break
    
while money>0:
  order.append(money)
  order.sort()
  i=order.index(money)
  if order.count(money)>1:
    owed=money
    count[owed]+=1
    money=0
  else:
    owed=order[i-1]
    count[owed]+=1
    order.remove(money)
    money-=owed

print("Changes to be returned")
print("-"*24)

for key,val in count.items():
  if not val==0:
    print(f"{change[key]} : {val}")