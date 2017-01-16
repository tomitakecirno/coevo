"""
雑感
今は純粋に勝数をカウントして個体を残している.
せっかく相手個体毎に勝数をカウントできてるんだからそれを統計学的なものを利用してどうにかしたい
先攻後攻で特定の少数の相手に全部勝てている個体よりも,先攻後攻のどちらかの勝利でもたくさんの相手に
勝てている個体の方が評価値は高くなると思われる.

じゃあ次は,勝数×相手の勝利数にしてみる.
"""
import AnsModule
import Config
import nim
import random

def InitPop(Pop):
  for i in Pop:
    for j in range(1,6):
      if j == 2:
        i[j] = 0
      else :
        i[j] = []

#近傍リスト生成してニッチ割り振って主親と副親を決める
def Setting(Pop):
  #リスト生成
  AnsModule.List1(Pop)
  AnsModule.List2(Pop)
  #ニッチ割り振り
  Config.CountNitch = 1
  for i in range(len(Pop)):
    if AnsModule.SetNitch(i,Pop,Config.CountNitch) == 1:
      Config.CountNitch += 1
  Config.CountNitch-=1
  #主親を選ぶ
  MainPare_n = random.randint(0,len(Pop)-1) #主親を選ぶ
  #副親を選ぶ
  SubPare_n = []
  appSub = SubPare_n.append
  for i in range(Config.Define.Np):
    TmpRand = random.randint(0,len(Pop[MainPare_n][3])-1)
    #追加と同時に要素を削除する
    appSub(Pop[MainPare_n][3].pop(TmpRand))
  #print(SubPare_n)
  return MainPare_n,SubPare_n
  
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
def Battle(Pop,Child,NimStatus):
  #主親以外の相手集団全員と対戦
  PopWin = 0
  ChildWin = 0
  BattleCount = 0
  #対戦相手集団を設定（インデックスのみ）
  for i in Pop:
    if len(i[0])!=0:
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
        """
        if PopWin == 2:
          PopWin+=1
        if ChildWin == 2:
          ChildWin+=1
        """
        appChild(ChildWin)
        appPop(PopWin)
        BattleCount+=2
    #print(PopWin,ChildWin)
  return BattleCount

def FitnessValueChild(Child,Opponent):
  #勝ちポイントの合計を入れておく
  tmpWin = [0 for i in range(len(Opponent))]
  for i in range(len(Opponent)):
    tmpWin[i] = sum(Opponent[i][1])
  #評価値に付け加えていく
  for i in Child:
    tmpFitness = 0
    for j in range(len(i[1])):
      tmpFitness+=i[1][j]*tmpWin[j]
    i[2] = tmpFitness
    
def FitnessValuePop(Pop,Opponent):
  #勝ちポイントの合計を入れておく
  tmpWin = [0 for i in range(len(Opponent))]
  for i in range(len(Opponent)):
    tmpWin[i] = sum(Opponent[i][1])
  #評価値に付け加えていく
  for i in Pop:
    tmpFitness = 0
    for j in range(len(i[1])):
      tmpFitness+=i[1][j]*tmpWin[j]
    i[5] = tmpFitness
        
def GetData(Child):
  TmpSum = []
  for i in Child:
    TmpSum.append(sum(i[1]))
  print("最大",max(TmpSum))
  print("平均",round(sum(TmpSum)/len(TmpSum),1))
  print("最小",min(TmpSum))

    
#集団1個、全数対戦させる手法（1個前の手法）
def coans(Pop,Main,Sub,NimStatus):
  #子個体取得
  Child = AnsModule.TwoPointCrossover(Pop,Main,Sub)
  #主親を一旦取り除いておく(tmpPopに主親を除いた集団を入れる)
  tmpPop = []
  appPop = tmpPop.append
  for i in Pop:
    if i!=Pop[Main]:
      appPop(i)
  #バトル
  Config.BattleCountBef+=Battle(tmpPop,Child,NimStatus)
  FitnessValueChild(Child,tmpPop)
  #勝ち数取得
  #TmpSum = [sum(i[1]) for i in Child]
  #TmpIndex = TmpSum.index(max(TmpSum))
  #一番高い適応度の個体を取得
  TmpFit = [i[2] for i in Child]
  TmpIndex = TmpFit.index(max(TmpFit))
  #もし主親よりも得点が高い個体がいた場合、主親と入れ替える
  if(TmpIndex != Config.Define.Nc):
    Pop[Main][0] = Child[TmpIndex][0]

#今の共進化ANS（集団1個, 各クラスタからランダムに1個体ランダムに選んで対戦させる方法）
def coans2(Pop,Main,Sub,NimStatus):
  #子個体取得
  Child = AnsModule.TwoPointCrossover(Pop,Main,Sub)
  #対戦相手集団生成
  Opponent = SelectOpponent(Pop,Main)
  #バトル
  Config.BattleCountNow+=Battle(Opponent,Child,NimStatus)
  FitnessValueChild(Child,Opponent)
  #勝ち数取得
  #TmpSum = [sum(i[1]) for i in Child]
  #TmpIndex = TmpSum.index(max(TmpSum))
  #一番高い適応度の個体を取得
  TmpFit = [i[2] for i in Child]
  TmpIndex = TmpFit.index(max(TmpFit))
  #もし主親よりも得点が高い個体がいた場合、主親と入れ替える
  if(TmpIndex != Config.Define.Nc):
    Pop[Main][0] =  Child[TmpIndex][0]




  
