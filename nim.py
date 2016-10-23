#ニム
#2人で行う/山は4つ
import config
import random
import ansmodule
  

#以下mainルーチン
NimData = config.Nim() #山生成
config.Nim.DataSet(NimData) #評価用配列に01を代入
config.Nim.test()

#集団生成 [戦略/各個体に対する勝ちポイント/ニッチ番号/リスト１/リスト２]
#Pop = [[0 if i==2 else [] for i in range(5)] for j in range(config.Define.Ns)]
#子個体集団生成 [戦略/各個体に対する勝ちポイント]
#Child = [[[] for i in range(2)] for j in range(config.Define.Nc)]

#仮の座標設定
#for i in range(len(Pop)):
#  Pop[i][0].append(random.randint(0,10))
#  Pop[i][0].append(random.randint(0,10))

#リスト生成  
#ansmodule.List1(Pop)
#ansmodule.List2(Pop)
#ニッチ割り振り
#config.CountNitch = 1
#for i in range(len(Pop)):
#  if ansmodule.SetNitch(i,Pop,config.CountNitch) == 1:
#    config.CountNitch += 1

#主親と副親を選ぶ
#MainPare_n = random.randint(0,len(Pop)-1) #主親を選ぶ
#SubPare_n = []
#for i in range(config.Define.Np):
#  TmpRand = random.randint(0,len(Pop[MainPare_n][3])-1)
#  SubPare_n.append(Pop[MainPare_n][3].pop(TmpRand))
#print(SubPare_n)

#subPare = []
#for i in range(config.Define.Np):
#  TmpRand = random.randint(0,len(Pop[MainPare_n][3]))

#for e in range(config.Define.END):
#  coans() #卒論のANS（集団2個）
#  coans2()  #今のANS（集団1個）
