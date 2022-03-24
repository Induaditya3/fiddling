# TRANSPORTATION PROBLEM
#NORTH WEST METHOD SOLUTION

class Cost:
  def __init__(self,cost,trip,val):
    self.cost=cost
    self.trip=trip
    self.val=val

        
supply=[50,75,25]#TO FILL
demand=[20,20,50,60]#TO FILL

tcost=[[3,5,7,6],[2,5,8,2],[3,6,9,2]]#TO FILL

#employing similar in spirit to array of structure to keep track of cost and trips concomitantly
costNtrip=[]    
for i in range(len(supply)):
  l=[]
  for j in range(len(demand)):
    a=Cost(tcost[i][j],0,0)
    l.append(a)
  costNtrip.append(l)

def stop(i,j):
  '''to check stop while loop'''
  global supply
  global demand
  return False if supply[i]==0 or demand[j]==0 else True
    
def min(i,j):
  '''to subtract smaller of supply and demand from larger ones and update trip every time that happens'''
  c=0
  global supply
  global demand
  global costNtrip 
  if supply[i]>demand[j]:
    supply[i]-=demand[j]
    costNtrip[i][j].val=demand[j]
    demand[j]-=demand[j]
    c=1
  else:
    demand[j]-=supply[i]
    costNtrip[i][j].val=supply[i]
    supply[i]-=supply[i]
    c=1
  costNtrip[i][j].trip+=c

#to traverse costNtrip and apply north west method

for i in range(len(supply)):
  for j in range(len(demand)):
    while stop(i,j):
      min(i,j)

def calc():
  '''it returns minimised cost by North West algorithm'''
  global costNtrip
  global copy
  totalcost=0
  for i in range(len(supply)):
    for j in range(len(demand)):
      if costNtrip[i][j].trip>0:
        totalcost+=costNtrip[i][j].cost*costNtrip[i][j].val
  return totalcost
  
#to display cost matrix
for i in range(len(supply)):
    for j in range(len(demand)):
      print(costNtrip[i][j].cost,end=' ')
    print()

print('\n'*3)    
#to print the trips made
for i in range(len(supply)):
    for j in range(len(demand)):
      print(costNtrip[i][j].trip,end=' ')
    print()

print("Minimised transportation cost =",calc())   