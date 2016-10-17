from enum import IntEnum
import random
class Define(IntEnum):
  Ns = 3    #初期集団数
  No = 30    #敵集団数
  Np = 4     #親個体数
  Nc = 5     #子個体数
  DEM = 2    #次元数
  Kp = 3
  Ko = 3
  T = 1      #ステップサイズ

#Nimの山に関するクラス
class Nim:
  _MtNumber = 4
  Stone = list([3]*_MtNumber)
  Eval = []

class Kotai:
  n = []
  win = 0
  nitch = 0
  List1 = list([Define.Kp])
  List2 = list([Define.Ko])
