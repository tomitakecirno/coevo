#ニム
#2人で行う/山は4つ
import Config
import random
import AnsModule
import numpy as np

def DataSet(NimData):
  for x in range(NimData.Stone[0]+1):
    for y in range(NimData.Stone[1]+1):
      for z in range(NimData.Stone[2]+1):
        for a in range(NimData.Stone[3]+1):
          if bin(x^y^z^a)!='0b0':
            NimData.Eval.append(1)
          else :
            NimData.Eval.append(0)
  #print(len(NimData.Eval))

    
NimData = Config.Nim() #山生成
DataSet(NimData) #評価用配列に01を代入 これが多分最適解となる

Pop = [[0 if i==1 or i==2 else [] for i in range(5)] for j in range(5)]
for i in range(len(Pop)):
  Pop[i][0].append(random.randint(0,10))
  Pop[i][0].append(random.randint(0,10))
  
Child = [[0 if i==1 or i==2 else [] for i in range(5)] for j in range(5)]
AnsModule.List2(Pop)
MainPare_n = random.randint(0,len(Pop)) #主親を選ぶ

