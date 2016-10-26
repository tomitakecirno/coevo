import AnsModule
import Config
import nim
import random

def InitPop(Pop):
  for i in Pop:
    for j in range(1,5):
      if j == 2:
        i[j] = 0
      else :
        i[j] = []
        
#各クラスタの中から個体を1個選んで対戦相手とする
def SelectOpponent(Pop,Main):
  TmpOpponentIndex = []
  TmpOpponent = []
  #ニッチの数だけリストを作る
  TmpList = [[] for i in range(Config.CountNitch)]
  for i in range(len(Pop)):
    if i != Main:
      TmpList[Pop[i][2]-1].append(i)
  for i in TmpList:
    #主親の属するニッチが主親だけの場合空リストとなるので例外処理
    if len(i)!=0:
      TmpOpponentIndex.append(random.choice(i))
  for i in TmpOpponentIndex:
    TmpOpponent.append(Pop[i])
  return TmpOpponent
  
#戦わせる
def Battle(Pop,Child,Main,NimStatus):
  #主親以外の相手集団全員と対戦
  PopWin = 0
  ChildWin = 0
  #対戦相手集団を設定（インデックスのみ）
  for i in Pop:
    if len(Pop[0])!=0:
      appPop = i[1].append
      for j in Child:
        PopWin = 0
        ChildWin = 0
        appChild = j[1].append
        #1戦目
        NimStatus = nim.NimInit()
        if nim.NimGame(i,j,NimStatus) == 0:
          PopWin+=1
        else :
          ChildWin+=1
        #2戦目
        NimStatus = nim.NimInit()
        if nim.NimGame(j,i,NimStatus) == 0:
          ChildWin+=1
        else :
          PopWin+=1
        if PopWin == 2:
          PopWin+=1
        if ChildWin == 2:
          ChildWin+=1
        appChild(ChildWin)
        appPop(PopWin)
    #print(PopWin,ChildWin)

def GetData(Child):
  TmpSum = []
  for i in Child:
    TmpSum.append(sum(i[1]))
  print("最大",max(TmpSum))
  print("平均",round(sum(TmpSum)/len(TmpSum),1))
  print("最小",min(TmpSum))

    
#集団1個、全数対戦させる手法
def coans(Pop,Main,Sub):
  #子個体取得
  Child = AnsModule.TwoPointCrossover(Pop,Main,Sub)
  #バトル
  Battle(Pop,Child,Main,NimStatus)
  TmpSum = [sum(i[1]) for i in Child]
  #勝ち数取得
  TmpIndex = TmpSum.index(max(TmpSum))
  #もし主親よりも得点が高い個体がいた場合、主親と入れ替える
  if(TmpIndex != Config.Define.Nc):
    Pop[Main][0] =  Child[TmpIndex][0]
#今の共進化ANS（集団1個）
def coans2(Pop,Main,Sub,NimStatus):
  #子個体取得
  Child = AnsModule.TwoPointCrossover(Pop,Main,Sub)
  #バトル
  Opponent = SelectOpponent(Pop,Main)
  Battle(Opponent,Child,Main,NimStatus)
  TmpSum = [sum(i[1]) for i in Child]
  #勝ち数取得
  TmpIndex = TmpSum.index(max(TmpSum))
  #もし主親よりも得点が高い個体がいた場合、主親と入れ替える
  print("ChildData")
  GetData(Child)
  if(TmpIndex != Config.Define.Nc):
    Pop[Main][0] =  Child[TmpIndex][0]




  
