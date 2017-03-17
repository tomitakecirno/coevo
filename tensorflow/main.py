"""""""""""""""""""""""
メイン関数のつもり
"""""""""""""""""""""""
import Csv_Module
import sys

if __name__ == "__main__":
  args = sys.argv #コマンドライン引数
  if len(args) <= 1:
    print("No Input Files")
  else :
    Csv_Module.readfile(args[1]) #csvファイル読み込み
    