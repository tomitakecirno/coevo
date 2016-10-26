import Config

def NimInit():
  #ニムの石の個数や状態を格納するリスト生成
  #NimStatus[0]:それぞれの山の石の数
  #NimStatus[1]:評価用配列
  #NimStatus[2]:評価用配列に対応する山の石の数
  TmpNimStatus = [[] for j in range(3)]
  
  TmpNimStatus[0].append(Config.Define.Poll1)
  TmpNimStatus[0].append(Config.Define.Poll2)
  TmpNimStatus[0].append(Config.Define.Poll3)
  #遷移可能な状態を0or1で設定する
  TmpNimStatus[1] = InitDataSet(TmpNimStatus[0])
  TmpNimStatus[2] = DataSet2()
  return TmpNimStatus

#ニムのポールリストから遷移可能な状態を0or1で格納
def InitDataSet(NimPoll):
  TmpNimData2 = []
  #0で初期化
  app2 = TmpNimData2.append
  for z in range(Config.Define.Poll3+1): #2
    for y in range(Config.Define.Poll2+1): #1
      for x in range(Config.Define.Poll1+1): #0
        if (x == NimPoll[0] and y == NimPoll[1]) or (y == NimPoll[1] and z == NimPoll[2]) or (x == NimPoll[0] and z == NimPoll[2]):
          app2(1)
        else :
          app2(0)
  return TmpNimData2
  
def DataSet(NimPoll):
  TmpNimData2 = []
  #0で初期化
  app2 = TmpNimData2.append
  for z in range(Config.Define.Poll3+1): #2
    for y in range(Config.Define.Poll2+1): #1
      for x in range(Config.Define.Poll1+1): #0
        if (x == NimPoll[0] and y == NimPoll[1] and z < NimPoll[2]) or (x < NimPoll[0] and y == NimPoll[1] and z == NimPoll[2]) or (x == NimPoll[0] and y < NimPoll[1] and z == NimPoll[2]):
          app2(1)
        else :
          app2(0)
  return TmpNimData2
  
#評価用配列に対応する山の石の数をぶち込む
def DataSet2():
  TmpNimData3 = []
  app3 = TmpNimData3.append
  for z in range(Config.Define.Poll3+1): #2
    for y in range(Config.Define.Poll2+1): #1
      for x in range(Config.Define.Poll1+1): #0
        app3([x,y,z])
  return TmpNimData3

#最適戦略を求めておく
def Optimaze():
  TmpOpt = []
  #0で初期化
  app2 = TmpOpt.append
  for z in range(Config.Define.Poll3+1): #2
    for y in range(Config.Define.Poll2+1): #1
      for x in range(Config.Define.Poll1+1): #0
        if bin(x^y^z)!='0b0':
          app2(1)
        else :
          app2(0)
  return TmpOpt

#Nimの石の数の操作 n:配列のインデント Nimの状態
def NimGetout(n,NimStatus):
  #次の石の数を取得
  NimStatus[0] = NimStatus[2][n]
  NimStatus[1] = DataSet(NimStatus[0])
#先攻は2点,後攻は1.5点
def NimGame(Pop,Child,NimStatus):
  #解集団が先攻、子個体集団が後攻
  Config.BattleCount+=1
  winflag = 0
  while sum(NimStatus[0]):
    #評価用配列をスキャン、両方1になる部分ところを選ぶ
    check = 0
    #先攻
    for i in range(len(NimStatus[1])):
      if Pop[0][i] == 1 and NimStatus[1][i] == 1:
        #遷移可能な状態が戦略内にあればチェック
        check = 1
        #次の山の状態を取得
        NimGetout(i,NimStatus)
        if sum(NimStatus[0]) == 0:
          winflag = 0 #先攻が勝利
        break
    #もしマッチする戦略がない場合,設定可能な部分で一番最初にヒットする戦略を取る
    if check == 0:
      for i in range(len(NimStatus[1])):
        if NimStatus[1][i] == 1:
          NimGetout(i,NimStatus)
          if sum(NimStatus[0]) == 0:
            winflag = 0 #先攻が勝利
    #後攻
    if(sum(NimStatus[0]) == 0):
      break;
    check = 0
    for i in range(len(NimStatus[1])):
      if Child[0][i] == 1 and NimStatus[1][i] == 1:
        #遷移可能な状態が戦略内にあればチェック
        check = 1
        #次の山の状態を取得
        NimGetout(i,NimStatus)
        if sum(NimStatus[0]) == 0:
          winflag = 1 #後攻が勝利
        break
    #もしマッチする戦略がない場合,設定可能な部分で一番最初にヒットする戦略を取る
    if check == 0:
      for i in range(len(NimStatus[1])):
        if NimStatus[1][i] == 1:
          NimGetout(i,NimStatus)
          if sum(NimStatus[0]) == 0:
            winflag = 1 #後攻が勝利
    if(sum(NimStatus[0]) == 0):
      break;
  return winflag














  
