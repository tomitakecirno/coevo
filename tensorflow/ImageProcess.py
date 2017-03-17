""""""""""""""""""""""""""""""""""
迫真画像処理部～顔画像トリミングの裏技～
入力はフォルダー
"""""""""""""""""""""""""""""""""
import cv2
import sys
import os
import shutil
import glob

args = sys.argv

#例外処理
if len(args) <= 1:
  print("No Input Files")
  quit()
#ファイルのパス取得
file_path = args[1] + '/*'
dir_image = glob.glob(file_path)

#保存用のフォルダ作成
save_path = args[1]+'_face'
if os.path.isdir(save_path):
  shutil.rmtree(save_path)
os.mkdir(save_path)

#カスケード読み込み
cascade_path = "lbpcascade_animeface.xml"

i=0
for dir in dir_image:
  print(dir)
  #ファイル読み込み
  image = cv2.imread(dir)
  #例外処理
  if(image is None):
    print("画像を開けません")
    quit()
  #グレースケール変換
  image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
  #ヒストグラム平坦化
  image_equal = cv2.equalizeHist(image_gray)

  #カスケードの特徴量を取得する
  cascade = cv2.CascadeClassifier(cascade_path)
  facerect = cascade.detectMultiScale(image_equal, scaleFactor=1.2, minNeighbors=2, minSize=(80,80))
  
  tag = os.path.splitext(dir)[-1]
  for rect in facerect:
    #顔だけ切り出して保存
    x = rect[0]
    y = rect[1]
    width = rect[2]
    height = rect[3]
    #切り取る範囲指定
    dst = image[y:y+height, x:x+width]
    new_image_path = save_path + '/' + "{0:03d}".format(i) + tag
    cv2.imwrite(new_image_path, dst)
    i+=1