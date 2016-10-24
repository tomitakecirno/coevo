from enum import IntEnum
import random

CountNitch = 0

class Define(IntEnum):
  Ns = 5     #初期集団数
  No = 30    #敵集団数
  Np = 3     #親個体数
  Nc = 3     #子個体数
  DEM = 2    #次元数
  Kp = 3
  Ko = 3
  T = 1      #ステップサイズ
  END = 1  #終了時間

#Nimの山に関するクラス
class Nim:
  _MtNumber = 4
  Stone = list([3]*_MtNumber)
  Eval = []
  
  def DataSet(NimData):
    for x in range(NimData.Stone[0]+1):
      for y in range(NimData.Stone[1]+1):
        for z in range(NimData.Stone[2]+1):
          for a in range(NimData.Stone[3]+1):
            if bin(x^y^z^a)!='0b0':
              NimData.Eval.append(1)
            else :
              NimData.Eval.append(0)
  def test():
    print(_MtNumber) 
  #print(len(NimData.Eval))

class Kotai:
  n = []
  win = 0
  nitch = 0
  List1 = [[] for i in range(5)]
  List2 = []
