"""
ニム
2人で行う/山は4つ
popの仕様
[0]解のベクトル(0or1)
[1]勝ちポイント
[2]ニッチ番号
[3]リスト1
[4]リスト2
[5]適応度（評価値）
childの仕様
[0]解のベクトル(0or1)
[1]勝ちポイント
[2]適応度（評価値）
"""
import Config
import random
import gmodel
import nim
import image
for h in range(Config.Define.TRY):
  random.seed()
  #Config.BattleCountBef = 0
  Config.BattleCountNow = 0
  #Config.BattleCountSotsu = 0
  NimStatus = nim.NimInit()
  save = NimStatus[1]
  #集団生成
  PopNow = gmodel.InitNumbers(Config.Define.Ns) #現手法
  #PopBef = gmodel.Init26(Config.Define.Ns)
  #PopNow = gmodel.Init26(Config.Define.Ns,NimStatus[1]) #現手法
  #PopSotsu = gmodel.Init26(Config.Define.Ns) #卒論手法
  #OpoSotsu = gmodel.Init26(Config.Define.No) #卒論手法
  for e in range(Config.Define.END):
    #if e%100 == 0:
      #print("世代数:",e)
    #卒論
    #MainSotsu,SubSotsu = gmodel.Setting(PopSotsu)
    #damy,damy2 = gmodel.Setting(OpoSotsu)
    #前手法
    #MainBef,SubBef = gmodel.Setting(PopBef)
    #現手法
    gmodel.Setting(PopNow)
    image.ShowNitch(PopNow)
    #手法切り替え
    #gmodel.coansSotsu(PopSotsu,OpoSotsu,MainSotsu,SubSotsu,NimStatus) #卒論のCOANS（集団2個）
    #gmodel.coans(PopBef,MainBef,SubBef,NimStatus)  #一個前のCOANS
    #gmodel.coans2(PopNow,MainNow,SubNow,NimStatus)  #今のCOANS（集団1個）
    #初期化
    #if e != Config.Define.END-1:
     # gmodel.InitPop(PopNow)
      #gmodel.InitPop(PopBef)
      #gmodel.InitPop(PopSotsu)
      #gmodel.InitPop(OpoSotsu)
  #複製
  """
  subPopSotsu = [[0 if i==2 or i==5 else [] for i in range(6)] for j in range(Config.Define.Ns)]
  subPopNow = [[0 if i==2 or i==5 else [] for i in range(6)] for j in range(Config.Define.Ns)]
  for i in range(Config.Define.Ns):
    #subPopSotsu[i][0] = PopSotsu[i][0]
    subPopNow[i][0] = PopNow[i][0]
    
  #一旦リセットしてベスト個体を求める
  gmodel.InitPop(PopNow)
  gmodel.InitPop(subPopNow)
  gmodel.Battle(PopNow,subPopNow,NimStatus)
  gmodel.FitnessValuePop(PopNow,subPopNow)
  BestIndexPopNow = nim.GetBestIndex(PopNow,5) #ベスト個体のインデックス
  #print("Best:",PopNow[BestIndexPop][5])
  #卒論
  gmodel.InitPop(PopSotsu)
  gmodel.InitPop(subPopSotsu)
  gmodel.Battle(PopSotsu,subPopSotsu,NimStatus)
  gmodel.FitnessValuePop(PopSotsu,subPopSotsu)
  BestIndexPopSotsu = nim.GetBestIndex(PopSotsu,5) #ベスト個体のインデックス

  #卒論が前,現手法が後
  BestIndiv = []
  appBestIndiv = BestIndiv.append
  appBestIndiv(PopSotsu[BestIndexPopSotsu])
  appBestIndiv(PopNow[BestIndexPopNow])

  f = open('Result.dat','a')
  f.write('試行回数,' + str(h) + '\n')
  f.write('卒論の集団と比較:' + '\n')
  gmodel.InitPop(BestIndiv)
  gmodel.InitPop(subPopSotsu)
  gmodel.Battle(BestIndiv,subPopSotsu,NimStatus)
  f.write('卒論手法,'+str(BestIndiv[0][1].count(2)/len(subPopSotsu))+','+str(BestIndiv[0][1].count(2))+'\n')
  f.write('現手法,'+str(BestIndiv[1][1].count(2)/len(subPopSotsu))+','+str(BestIndiv[1][1].count(2))+'\n')

  f.write('現手法の集団と比較,' + '\n')
  gmodel.InitPop(BestIndiv)
  gmodel.InitPop(subPopNow)
  gmodel.Battle(BestIndiv,subPopNow,NimStatus)
  f.write('卒論手法,'+str(BestIndiv[0][1].count(2)/len(subPopNow))+','+str(BestIndiv[0][1].count(2))+'\n')
  f.write('現手法,'+str(BestIndiv[1][1].count(2)/len(subPopNow))+','+str(BestIndiv[1][1].count(2))+'\n')

  f.write('卒論手法対戦数,' + str(Config.BattleCountSotsu) + '\n')
  f.write('現手法対戦数,' + str(Config.BattleCountNow) + '\n')
  f.close
  """
  """
  NitchBef = []
  appNitchBef = NitchBef.append
  for i in PopBef:
    appNitchBef(i[2])

  ResultBef = nim.CreateResult(PopBef)
  ResultNow = nim.CreateResult(PopNow)
  #複製
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

  print("前手法対戦数:",Config.BattleCountBef)
  print("現手法対戦数:",Config.BattleCountNow)
  print("前手法ニッチ数:",max(NitchBef))
  print("現手法ニッチ数:",max(NitchNow))
  """











  
  



