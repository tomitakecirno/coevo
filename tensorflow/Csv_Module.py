"""""""""""""""""""""""""""""""""
csvファイルを処理するモジュール的なもの
"""""""""""""""""""""""""""""""""
import pandas as pd

def readfile(filename):
  df = pd.read_csv(filename)
  print(df)
  #print(df.ix[0,0])
