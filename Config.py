from enum import IntEnum

CountNitch = 0

class Define(IntEnum):
  Ns = 5     #初期集団数
  No = 30    #敵集団数
  Np = 3     #親個体数
  Nc = 3     #子個体数
  DEM = 1    #次元数
  Kp = 3
  Ko = 3
  T = 1      #ステップサイズ
  END = 1  #終了時間
  Poll1 = 1
  Poll2 = 1
  Poll3 = 1

#Nimの山に関するクラス
class Nim:
  _MtNumber = 4
  Stone = list([3]*_MtNumber)
  #print(len(NimData.Eval))

class Kotai:
  n = []
  win = 0
  nitch = 0
  List1 = [[] for i in range(5)]
  List2 = []
