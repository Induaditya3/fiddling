letters=tuple([chr(x) for x in range(ord('a'),ord('z')+1)]+[chr(X) for X in range(ord('A'),ord('Z')+1)])
string="It was a bright cold day in April, and the clocks were striking thirteen. Winston Smith, his chin nuzzled into his breast in an effort to escape the vile wind, slipped quickly through the glass doors of Victory Mansions, though not quickly enough to prevent a swirl of gritty dust from entering along with him."
letter=0
sentences=0
for i in string:
  if i in letters:
    letter+=1
  elif i=='.':
    sentences+=1
words=len(string.split())
L=letter*100/words
S=sentences*100/words
index = round(0.0588*L-0.296*S-15.8)
if index<1:
  print("Before Grade 1")
elif index<16:
  print("Grade",index)
else:
  print("Grade 16+")