# -*- coding: utf-8 -*-
"""""""""""""""""""""""""""
迫真画像処理部～補正の秘技～
256^3→64へ減色して特徴量ベクトルを生成してCSVへ出力するゾ
特徴量は色。64色あるから64次元
"""""""""""""""""""""""""""
import cv2
import os
import glob
import numpy as np
import pandas as pd

cascade_path = "lbpcascade_animeface.xml"

def normalize(img):
  norm = np.linalg.norm(img)
  image_nor = cv2.normalize(img,norm, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX)
  return image_nor

def decleaseColor(img):
  rgb = cv2.split(img)
  for col in rgb:
    idx = np.where(col < 64)
    col[idx] = 32
    idx = np.where((64<=col) & (col<128))
    col[idx] = 96
    idx = np.where((128<=col) & (col<196))
    col[idx] = 160
    idx = np.where(196<=col)
    col[idx] = 224
  d_img = cv2.merge(rgb)
  return d_img

def CreateFeature(img):
  train = [0 for i in range(64)]
  for cols in img[0:1]:
    for rows in cols:
      color = int(rows[0]/64) + int(rows[1]/64)*4 + int(rows[2]/64)*16
      train[color]+=1
  return train

def Output_Csv(DataSet):
  df = pd.DataFrame(DataSet)
  df.to_csv("train.csv",header=False,index=False)
  
#フォルダからファイルを読み込んで減色の後，特徴量ベクトルを生成してcsvファイルへ
def ImageToFeature(dn):
  DataSet = []
  app_ds = DataSet.append
  for dp in range(dn):
    dir_path = './training/' + str(dp)
    if os.path.isdir(dir_path):
      pass
    else :
      print("folder:",dp," ..Not such a directry")
      continue
    
    file_path = glob.glob(dir_path+'/*')
    if len(file_path):
      save_path = dir_path + '_face'
      if os.path.isdir(save_path):
        pass
      else :
        os.mkdir(save_path)
    else :
      print("File is not exist.")
      continue
    #正規化と減色
    i=0
    for fp in file_path:
      #print(fp)
      image = cv2.imread(fp)
      tag = os.path.splitext(fp)[-1]
      #正規化するっぽい
      d_image = decleaseColor(image)
      trainData = CreateFeature(d_image)
      trainData.append(dp)
      app_ds(trainData)
      new_file_path = save_path + '/' + "{0:03d}".format(i) + tag
      cv2.imwrite(new_file_path,d_image)
      i+=1
    print("folder:",dp,'  ',i,"files")
  Output_Csv(DataSet)
  
#メインルーチン
ImageToFeature(10)