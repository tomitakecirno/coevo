#ニム
#2人で行う/山は4つ
import Config
import random
import AnsModule
import gmodel
import nim

if __name__ == '__main__':
  NimStatus = nim.NimInit()
  save = NimStatus[1]
  #集団生成 [戦略/各個体に対する勝ちポイント/ニッチ番号/リスト１/リスト２]
  Pop = [[0 if i==2 else [] for i in range(5)] for j in range(Config.Define.Ns)]
  #戦略設定
  for i in Pop:
    i[0] = [random.randint(0,1) for j in range(len(NimStatus[1]))]
  for e in range(Config.Define.END):
    #リスト生成
    AnsModule.List1(Pop)
    AnsModule.List2(Pop)
    #ニッチ割り振り
    Config.CountNitch = 1
    for i in range(len(Pop)):
      if AnsModule.SetNitch(i,Pop,Config.CountNitch) == 1:
        Config.CountNitch += 1
    Config.CountNitch-=1
    #主親と副親を選ぶ
    MainPare_n = random.randint(0,len(Pop)-1) #主親を選ぶ
    SubPare_n = []
    for i in range(Config.Define.Np):
      TmpRand = random.randint(0,len(Pop[MainPare_n][3])-1)
      SubPare_n.append(Pop[MainPare_n][3].pop(TmpRand))
      
    #手法切り替え
    #gmodel.coans(Pop,MainPare_n,SubPare_n,NimStatus) #卒論のANS（集団2個）
    gmodel.coans2(Pop,MainPare_n,SubPare_n,NimStatus)  #今のANS（集団1個）
    #初期化
    if e != Config.Define.END-1:
      gmodel.InitPop(Pop)
  print("PopData:")
  gmodel.GetData(Pop)
  Result = [[[] for i in range(3) ] for j in range(Config.CountNitch)]
  Count = [0 for i in range(1,Config.CountNitch+1)]
  TmpSum = [[[] for i in range(2)] for j in range(Config.CountNitch)]
  Opt = nim.Optimaze()
  for i in Pop:
    Count[i[2]-1]+=1

    #TmpSum[i[2]-1][0].append(i[0])
    #TmpSum[i[2]-1][1].append(sum(i[1]))
  """
  for i in range(len(TmpSum)):
    TmpIndex = TmpSum[i][1].index(max(TmpSum[i][1]))
    Result[i][0] = TmpSum[i][0][TmpIndex]
    Result[i][1] = TmpSum[i][1][TmpIndex]
  #print(AnsModule.HammingDis(Result[1][0],Opt))
  for i in Result:
    print("戦略:",i[0])
    print("最適:",Opt)
    print("得点:",i[1])
    print("\n")
  """
  print("ニッチの個数",Config.CountNitch)
  print("ニッチ毎の個体数",Count)
  print("総対戦数",Config.BattleCount)

  
