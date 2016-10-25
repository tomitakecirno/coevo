#ニム
#2人で行う/山は4つ
import Config
import random
import AnsModule
import gmodel
import nim

if __name__ == '__main__':
  NimStatus = nim.NimInit()
  #print(NimStatus[1])
  #集団生成 [戦略/各個体に対する勝ちポイント/ニッチ番号/リスト１/リスト２]
  Pop = [[0 if i==2 else [] for i in range(5)] for j in range(Config.Define.Ns)]
          
  #仮の座標設定
  for i in range(len(Pop)):
    Pop[i][0].append(random.randint(0,10))
    
  for e in range(Config.Define.END):
    #リスト生成
    AnsModule.List1(Pop)
    AnsModule.List2(Pop)
    #ニッチ割り振り
    Config.CountNitch = 1
    for i in range(len(Pop)):
      if AnsModule.SetNitch(i,Pop,Config.CountNitch) == 1:
        Config.CountNitch += 1
    #主親と副親を選ぶ
    MainPare_n = random.randint(0,len(Pop)-1) #主親を選ぶ
    SubPare_n = []
    for i in range(Config.Define.Np):
      TmpRand = random.randint(0,len(Pop[MainPare_n][3])-1)
      SubPare_n.append(Pop[MainPare_n][3].pop(TmpRand))
      #print(SubPare_n)
    
    #手法切り替え
    #gmodel.coans(Pop,MainPare_n,SubPare_n) #卒論のANS（集団2個）
    gmodel.coans2(Pop,MainPare_n,SubPare_n,NimStatus)  #今のANS（集団1個）
    
