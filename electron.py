n=int(input("ENTER THE ATOMIC NO. OF THE ELEMENT:"))
def Auf(shell):
	'''it has subshells'''
	if shell=='s':
		shell=[[[0],[0]]]
	elif shell=='p':
	    shell=[[[0],[0]],[[0],[0]],[[0],[0]]]
	elif shell=='d':
		shell=[[[0],[0]],[[0],[0]],[[0],[0]],[[0],[0]],[[0],[0]]]
	elif shell=='f':			shell=[[[0],[0]],[[0],[0]],[[0],[0]],[[0],[0]],[[0],[0]],[[0],[0]],[[0],[0]]]
	else:
		raise Exception("No such sell.")
	return shell

sscript=["",f"\N{SUPERSCRIPT ONE}",f"\N{SUPERSCRIPT TWO}",f"\N{SUPERSCRIPT THREE}",f"\N{SUPERSCRIPT FOUR}",f"\N{SUPERSCRIPT FIVE}",f"\N{SUPERSCRIPT SIX}",f"\N{SUPERSCRIPT SEVEN}",f"\N{SUPERSCRIPT EIGHT}",f"\N{SUPERSCRIPT NINE}"]

def elek(l):
    c=0
    for i in range(len(l)):
        for j in range(2):
            if l[i][j]==['up'] or l[i][j]==['down']:
                c+=1
    return c
  
orbital=('1s','2s','2p','3s','3p','4s','3d','4p','5s','4d','5p','4f','5d','6p','7s')

track=0
while n>0:
    orb=orbital[track]
    auf=Auf(orb[1])
    for i in range(len(auf)):
      if n<=0:
        break
      else:
        auf[i][0].pop()
        auf[i][0].append('up')
        n-=1
    for i in range(len(auf)):
      if n<=0:
        break
      else:
        auf[i][1].pop()
        auf[i][1].append('down')
        n-=1
    track+=1
    print(orb,sscript[elek(auf)],end=' ')