import Config

def NimInit():
  #ニムの石の個数や状態を格納するリスト生成
  TmpNimStatus = [[] for j in range(2)]
  TmpNimStatus[0].append(Config.Define.Poll1)
  TmpNimStatus[0].append(Config.Define.Poll2)
  TmpNimStatus[0].append(Config.Define.Poll3)
  #遷移可能な状態を0or1で設定する
  TmpNimStatus[1] = DataSet(TmpNimStatus[0])
  return TmpNimStatus

#ニムのポールリストから遷移可能な状態を0or1で格納
def DataSet(NimPoll):
  #print(NimPoll)
  TmpNimData = []
  #0で初期化
  app = TmpNimData.append
  for z in range(Config.Define.Poll3+1): #2
    for y in range(Config.Define.Poll2+1): #1
      for x in range(Config.Define.Poll1+1): #0
        if (x == NimPoll[0] and y == NimPoll[1]) or (y == NimPoll[1] and z == NimPoll[2]) or (x == NimPoll[0] and z == NimPoll[2]):
          app(1)
        else :
          app(0)
  return TmpNimData
  
#遷移可能か
def CheckNim(Select):
  pass
  #Stone  
def NimGame(Pop,Child,NimStatus):
  pass
  """
  while(sum(NimStatus[0]):
  NimStatus = NimInit()
  while(sum(NimStatus[0]):
  NimStatus = NimInit()
  """