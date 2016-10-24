import Config
import math
import random

def EuclidDis(one,ano):
  cal_sum = 0;
  for i in range(Config.Define.DEM):
    cal_sum += (one[i] - ano[i])*(one[i] - ano[i])
  return round(math.sqrt(cal_sum),1)

def List1(Pop):
  TmpDis = [[] for i in range(len(Pop))]
  
  for i in range(len(Pop)):
    app = TmpDis[i].append
    for j in range(len(Pop)):
      if i != j:
        app(EuclidDis(Pop[i][0],Pop[j][0]))
      else:
        app(50)
    #print(TmpDis[i])
    for k in range(Config.Define.Kp):
      TmpIndex = TmpDis[i].index(min(TmpDis[i]))
      Pop[i][3].append(TmpIndex)
      TmpDis[i][TmpIndex] = 50
    #print(Pop[i][3])

def List2(Pop):
  TmpDis = [[] for i in range(len(Pop))]
  TmpList = [[] for i in range(len(Pop))]

  for i in range(len(Pop)):
    for j in range(len(Pop)):
      if i != j:
        TmpDis[i].append(EuclidDis(Pop[i][0],Pop[j][0]))
      else:
        TmpDis[i].append(50)
    #print(TmpDis[i])
    for k in range(Config.Define.Kp):
      TmpIndex = TmpDis[i].index(min(TmpDis[i]))
      TmpList[i].append(TmpIndex)
      TmpDis[i][TmpIndex] = 50
    #print(TmpList[i])
  for i in range(len(Pop)):
    for j in range(len(TmpList[i])):
      TmpIndex = TmpList[i][j]
      if i in TmpList[TmpIndex]:
        Pop[i][4].append(TmpIndex)
      else :
        pass
    #print(Pop[i][4])

#ニッチ番号を割り当てる　未完成
def SetNitch(kn,Pop,n):
  #ニッチに属していなければニッチ番号を割り振る
  if Pop[kn][2] == 0:
    Pop[kn][2] = n
		#近傍リストを見ていく
    for j in range(len(Pop[kn][4])):
      TmpNumber = Pop[kn][4][j]
      SetNitch(TmpNumber,Pop,n)
    return 1
  else :
    return 0

#2点交叉の関数。subpare_nはリスト
def KousaOf2Point(Pop,Main,Sub):
  TmpChild = [[[] for i in range(2)] for j in range(Config.Define.Nc+1)]
  for i in range(Config.Define.Nc):
    TmpSub = random.choice(Sub)
    app = TmpChild[i][0].append
    for j in range(Config.Define.DEM):
      app((Pop[Main][0][j]+Pop[TmpSub][0][j])/2)
      
  TmpChild[Config.Define.Nc][0] = [x for x in Pop[Main][0]]
  return TmpChild
  
  
  
  
  
  
  
  
  
  
  
  
