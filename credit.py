def sum(a):
  return (a//10)+(a%10)
card_digits=15,13,16
while True:
  n=input('Enter the credit card number:')
  l=len(n)
  no_of_digits=l in card_digits
  if no_of_digits and n.isdigit():
    f_two=n[:2]
    n=int(n)
    break   
s=0
s2=0
for i in range(l):
  if i%2==1:
    s+=sum((n%10)*2)
    n//=10
  else:
    s2+=(n%10)
    n//=10
checksum=(s+s2)%10
check=False

card={("34","37"):f"AMEX\n",("51","52","53","54","55"):f"MASTERCARD\n",tuple(str(i) for i in range(40,50)):
f"VISA\n"}

for i in card:
  if f_two in i:
    check=True
    name=card[i]
    break
    
if checksum==0 and check:
  print(name)
else:
  print('INVALID')