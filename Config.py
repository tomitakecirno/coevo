from enum import IntEnum

CountNitch = 0
BattleCount = 0
BattleCountBef = 0
BattleCountNow = 0

class Define(IntEnum):
  Ns = 50    #初期集団数
  No = 10    #敵集団数
  Np = 3     #親個体数
  Nc = 10     #子個体数
  DEM = 2    #次元数
  Kp = 3
  Ko = 3
  T = 1      #ステップサイズ
  END = 50  #終了時間
  Poll1 = 5
  Poll2 = 5
  Poll3 = 5

#Nimの山に関するクラス
"""
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
"""
