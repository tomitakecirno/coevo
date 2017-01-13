"""
ニム
2人で行う/山は4つ
popの仕様
[0]解のベクトル(0or1)
[1]勝ちポイント
[2]ニッチ番号
[3]リスト1
[4]リスト2
"""
import Config
import random
import AnsModule
import gmodel
import nim

for h in range(1):
  Config.BattleCountBef = 0
  Config.BattleCountNow = 0
  NimStatus = nim.NimInit()
  save = NimStatus[1]
  #集団生成
  PopBef = [[0 if i==2 else [] for i in range(5)] for j in range(Config.Define.Ns)]
  PopNow = [[0 if i==2 else [] for i in range(5)] for j in range(Config.Define.Ns)]
  #戦略設定
  for i in PopBef:
    i[0] = [random.randint(0,1) for j in range(len(NimStatus[1]))]
  for i in PopNow:
    i[0] = [random.randint(0,1) for j in range(len(NimStatus[1]))]
  for e in range(Config.Define.END):
    #print("世代数:",e)
    MainBef,SubBef = gmodel.Setting(PopBef)
    MainNow,SubNow = gmodel.Setting(PopNow)
    #print(MainBef,SubBef)
    #print(MainNow,SubNow)
    #手法切り替え
    gmodel.coans(PopBef,MainBef,SubBef,NimStatus) #卒論のANS（集団2個）
    gmodel.coans2(PopNow,MainNow,SubNow,NimStatus)  #今のANS（集団1個）
    #初期化
    if e != Config.Define.END-1:
      gmodel.InitPop(PopNow)
      gmodel.InitPop(PopBef)
  #print("PopData:")
  #gmodel.GetData(Pop)
  """
  ResultBef = nim.CreateResult(PopBef)
  ResultNow = nim.CreateResult(PopNow)
  """
  subPopBef = PopBef
  subPopNow = PopNow
  #一旦リセット
  gmodel.InitPop(PopBef)
  gmodel.InitPop(PopNow)
  print("前手法vs前手法")
  nim.ShowResult(subPopBef,PopBef,NimStatus)  
  print("現手法vs前手法")
  nim.ShowResult(subPopNow,PopBef,NimStatus)  
  print("前手法vs現手法")
  nim.ShowResult(subPopBef,PopNow,NimStatus)  
  print("現手法vs現手法")
  nim.ShowResult(subPopNow,PopNow,NimStatus)  
  """
  #前手法,現手法それぞれで獲得した集団をそれぞれの集団と戦わせる
  #一旦リセット
  gmodel.InitPop(PopBef)
  gmodel.InitPop(PopNow)
  #一旦集団退避
  subPopBef = PopBef
  subPopNow = PopNow
  nim.ShowResult(PopNow,subPopBef,NimStatus)
  #一戦目
  #Battle(PopBef,subPopBef,NimStatus)
  #Battle(PopBef,subPopNow,NimStatus)

  #2セット目
  #Battle(PopNow,subPopBef,NimStatus)
  #Battle(PopNow,subPopNow,NimStatus)
  """
















  
  



