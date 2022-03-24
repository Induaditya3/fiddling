def gcd(a,b):
	'''recursive function'''
	if b==0:
		return a
	else:
		r=a%b
		return gcd(b,r)

print(gcd(25,75))