#ニム
#2人で行う/山は4つ
import random
import numpy as np
from enum import IntEnum

class define(IntEnum):
  Ns = 30    #初期集団数
  No = 30    #敵集団数
  Np = 4     #親個体数
  Nc = 5     #子個体数
  DEM = 2    #次元数
  Kp = 5
  Ko = 5
  T = 1      #ステップサイズ

#Nimの山に関するクラス
class Nim:
  _MtNumber = 4
  Stone = list([3]*_MtNumber)
  Eval = []

class Kotai:
  win = 0
  nitch = 0
  NeighList1 = list([define.Kp])
  NeighList2 = list([define.Ko])  
  pass

def DataSet(NimData):
  for x in range(NimData.Stone[0]+1):
    for y in range(NimData.Stone[1]+1):
      for z in range(NimData.Stone[2]+1):
        for a in range(NimData.Stone[3]+1):
          if bin(x^y^z^a)!='0b0':
            NimData.Eval.append(1)
          else :
            NimData.Eval.append(0)
  #print(len(NimData.Eval))
    
NimData = Nim()
DataSet(NimData) #評価用配列に01を代入
