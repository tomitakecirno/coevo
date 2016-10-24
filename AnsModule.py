import config
import math
import random

def EuclidDis(one,ano):
	cal_sum = 0;
	for i in range(config.Define.DEM):
		cal_sum += (one[i] - ano[i])*(one[i] - ano[i]);
		
	return round(math.sqrt(cal_sum),1);

def List1(Pop):
  TmpDis = [[] for i in range(len(Pop))]
  
  for i in range(len(Pop)):
    for j in range(len(Pop)):
      if i != j:
      	TmpDis[i].append(EuclidDis(Pop[i][0],Pop[j][0]))
      else:
      	TmpDis[i].append(50)
    #print(TmpDis[i])
    for k in range(config.Define.Kp):
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
    for k in range(config.Define.Kp):
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

def ExtensionXLM():
  pass
  #TmpChild = [[[] for i in range(2)] for j in range(config.Define.Nc)]
  #return TmpChild

def KousaOf2Point(MainPare_n,SubPare_n,Pop):
  pass
  #TmpChild = [[[] for i in range(2)] for j in range(config.Define.Nc)]
  #return TmpChild

def test():
  print(config.Pop)






  

