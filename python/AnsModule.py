import Config
import random
import math

def HammingDis(one,ano):
  cal_sum = 0;
  for (a,b) in zip(one,ano):
    if a!=b:
      cal_sum += 1
  return cal_sum

def EuclidDis(one,ano):
  cal_sum = 0;
  for (a,b) in zip(one,ano):
    cal_sum += (a-b)*(a-b)
  return math.sqrt(cal_sum)

def List1(Pop):
  TmpDis = [[] for i in range(len(Pop))]
  
  for i in range(len(Pop)):
    app = TmpDis[i].append
    for j in range(len(Pop)):
      if i != j:
        app(HammingDis(Pop[i][0],Pop[j][0]))
      else:
        app(1000)
    #print(TmpDis[i])
    for k in range(Config.Define.Kp):
      TmpIndex = TmpDis[i].index(min(TmpDis[i]))
      Pop[i][3].append(TmpIndex)
      TmpDis[i][TmpIndex] = 1000
    #print(Pop[i][3])

def List2(Pop):
  TmpList = MakeListDouble(Pop,Config.Define.Ko)
  for i in range(len(Pop)):
    for j in range(len(TmpList[i])):
      TmpIndex = TmpList[i][j]
      if i in TmpList[TmpIndex]:
        Pop[i][4].append(TmpIndex)
  for i in Pop:
    print(i[4])

def MakeListbit(Pop):
  TmpDis = [[] for i in range(len(Pop))]
  TmpList = [[] for i in range(len(Pop))]
  #総当たりで距離を保持する
  #自身のところには10000を代入
  #ついでに
  for i in range(len(Pop)):
    for j in range(len(Pop)):
      if i != j:
        TmpDis[i].append(HammingDis(Pop[i][0],Pop[j][0]))
      else:
        TmpDis[i].append(10000)
    for k in range(len(TmpDis[i])):
      TmpIndex = TmpDis[i].index(min(TmpDis[i]))
      TmpList[i].append(TmpIndex)
      TmpDis[i][TmpIndex] = 1000
  return TmpList

def MakeListDouble(Pop,k):
  TmpDis = [[] for i in range(len(Pop))]
  TmpList = [[] for i in range(len(Pop))]
  #総当たりで距離を保持する
  #自身のところには10000を代入
  #ついでに
  for i in range(len(Pop)):
    for j in range(len(Pop)):
      if i != j:
        TmpDis[i].append(EuclidDis(Pop[i][0],Pop[j][0]))
      else:
        TmpDis[i].append(10000)
    for j in range(k):
      TmpIndex = TmpDis[i].index(min(TmpDis[i]))
      TmpList[i].append(TmpIndex)
      TmpDis[i][TmpIndex] = 10000
  return TmpList
  
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
    
def SetNitchKai(Pop):
  count=1
  for i in range(len(Pop)):
    if Pop[i][2] == 0:
      tmpIndiv = []
      appIndiv = tmpIndiv.append
      appIndiv(i)
      for j in range(Config.Define.K2):
        for k in tmpIndiv:
          if len(Pop[k][4])!=0:
            for l in Pop[k][4]:
              #ニッチ番号が1,リスト内にないなら追加
              if Pop[l][2]==0 and l not in tmpIndiv:
                appIndiv(l)
                print("今のリスト:",tmpIndiv)
      for j in tmpIndiv:
        Pop[j][2] = count
      count+=1
  #for i in Pop:
  #  print(i[2])
  
#2点交叉の関数。subpare_nはリスト
def TwoPointCrossover(Pop,Main,Sub):
  TmpChild = [[0 if i==2 else [] for i in range(3)] for j in range(Config.Define.Nc+1)]
  #print(TmpChild[0])
  for i in range(Config.Define.Nc):
    #交叉を行う副親を決定
    TmpSub = random.choice(Sub)
    #一旦主親の評価用配列を入れる
    TmpBit = Pop[Main][0]
    #2点を決定して昇順にソート
    Tmp2p = [random.randint(0,len(Pop[Main][0])) for p in range(2)]
    Tmp2p = sorted(Tmp2p)
    #2点間を入れ替える    
    for j in range(Tmp2p[0],Tmp2p[1]):
      TmpBit[j] = Pop[TmpSub][0][j]
    TmpChild[i][0] = [k for k in TmpBit]
  TmpChild[Config.Define.Nc][0] = Pop[Main][0]
  Pop[Main][0] = []
  return TmpChild
  
  
