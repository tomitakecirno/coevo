import Config
import math

def EuclidDis(one,ano):
	cal_sum = 0;
	for i in range(Config.Define.DEM):
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
        
def MGG(one,ano,Child):
  pass





  

