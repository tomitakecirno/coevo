import AnsModule
import Config

#戦わせる
def Battle(Pop,Child,Main):
  for i in Pop:
    if i != Main:
      appi = i[1].append
      for j in Child:
        appj = j[1].append
        for x,y in zip(i[0],j[0]):
          appi(int(y<x));appj(int(x<y))
  """
  for i in Pop:
    print("Pop = ",i)
  for i in Child:
    print("Child = ",i)
  """  
def coans(Pop,Main,Sub):
  pass
  
#今の共進化ANS（集団1個）
def coans2(Pop,Main,Sub):
  """
  print("\n")
  for i in Pop:
    print("Pop = ",i)
  print("Mainインデックス:",Main)
  print("Main:",Pop[Main][0])
  """
  #子個体取得
  Child = AnsModule.KousaOf2Point(Pop,Main,Sub)
  #バトル
  Battle(Pop,Child,Main)
  TmpSum = [sum(i[1]) for i in Child]
  #勝ち数取得
  TmpIndex = TmpSum.index(max(TmpSum))
  if(TmpIndex != Config.Define.Nc):
    Pop[Main][0] = [x for x in Child[TmpIndex][0]]
  """
  print("結果")
  print("bestインデックス:",TmpIndex)
  print("best:",Child[TmpIndex][0])
  for i in Child:
    print("Child = ",i)
  for i in Pop:
    print("Pop = ",i)
  """
  #初期化