# -*- coding: utf-8 -*-
"""
ニューラルネットワークに通す.
Created on Thu Mar  9 18:17:05 2017
@author: tmtk
"""
import pandas as pd
import tensorflow as tf
import multiprocessing as mp
import sys

#csvから訓練データとラベルに分けて出力
def readfile(filename):
  df = pd.read_csv(args[1], header=None)
  training_set = df.ix[:,0:63].values.tolist()
  label = df.ix[:,64].values.tolist()
  return training_set,label

#以下メインルーチン
args = sys.argv
#例外処理
if len(args) < 2:
  print("No Input Files")
  quit()

training_set,label = readfile(args[1])

#入力層の定義
x = tf.placeholder(tf.float32,[None,64])

#入力層から中間層
#重みの設定
w_1 = tf.Variable(tf.truncated_normal([64,16],stddev=0.1), name="w1")
#中間層の設定と初期化
b_1 = tf.Variable(tf.zeros([16]),name="b1")
h_1 = tf.nn.relu(tf.matmul(x,w_1)+b_1)

#中間層から出力層
w_2 = tf.Variable(tf.truncated_normal([16,6], stddev=0.1), name="w2")
b_2 = tf.Variable(tf.zeros([6]), name="b2")
out = tf.nn.softmax(tf.matmul(h_1,w_2) + b_2)
"""
#誤差関数
loss = tf.reduce_mean(tf.square(y-out))
#訓練
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(loss)
#評価
correct = tf.equal(tf.argmax(out,1), tf.argmax(y,1))
accuracy = tf.reduce_mean(tf.cast(correct, tf.float32))

with tf.Session() as sess:
  #初期化
  init = tf.global_Variables_initializer()
  sess.run(init)
  
  #テストデータをロード
  for step in range(1000):
    sess.run(train_step,feed_dict={x:training_set, y:label})
    if step % 10 == 0:
      acc_val = sess.run(accuracy,feed_dict={x:training_set, y:label})
      print('Step %d: accuracy = %.2f' % (step,acc_val))
"""

