#program to convert infix to post fix
class Stack:
  def __init__(self,size):
    self.size=size
    self._l=[]
    self._top=-1
    
  def pop(self):
    if self._top>-1:
      self._top-=1
      popped=(self._l).pop()
      return popped
    raise Exception("Stack Underflow")
    
  def push(self,item):
    if self.size>(self._top+1):
      (self._l).append(item)
      self._top+=1
      return item
    raise Exception("Stack over flow")
    
  def peek(self):
    try:
      return self._l[self._top]
    except IndexError:
      return None
    
operator={"^":1,"*":2,"/":2,"+":3,"-":3,"(":4,")":4}

infix="a*(b+(c+d)*(e+f)/g)*h"

stk=Stack(len(infix))

expr=''

op=list(operator.keys())

for i in infix:
    if i not in op:
      expr+=i
    else:
      try:
        if i=="(":
          stk.push(i)
        elif i ==")":
          while True:
            if stk.peek()=="(":
              stk.pop()
              break
            else:
              expr+=stk.pop()
        elif stk._top>-1 and operator[stk.peek()]>operator[i]:
          stk.push(i)
        elif stk._top>-1 and operator[stk.peek()]==operator[i]:
          expr+=i
        else:
          while stk._top>-1 and operator[stk.peek()]<operator[i]:
            expr+=stk.pop()
          stk.push(i)
      except KeyError:
        pass

while True:
  if stk._top<0:
    break
  expr+=stk.pop()
print(expr)