#ニム
#2人で行う/山は3つ
import random
import numpy as np

#Nimの山に関するクラス
class Nim:
  _MtNumber = 4
  Stone = list([3]*_MtNumber)
  Eval = []
  

NimData = Nim()
for x in range(NimData.Stone[0]+1):
  for y in range(NimData.Stone[1]+1):
    for z in range(NimData.Stone[2]+1):
      for a in range(NimData.Stone[3]+1):
        if bin(x^y^z^a)!='0b0':
          NimData.Eval.append(1)
        else :
          NimData.Eval.append(0)
print(len(NimData.Eval))
#print(NimData.Eval)
