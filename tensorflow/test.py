import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from numpy.random import multivariate_normal, permutation
import pandas as pd
from pandas import DataFrame, Series

#トレーニング用のデータ
x = tf.placeholder(tf.float32, [None, 2])
print("training:",x)
#重み設定
w0 = tf.Variable(tf.zeros([2,1]))
w1 = tf.Variable(tf.zeros([1]))
print("parameter:",w1)
f = tf.matmul(x,w0)
#シグモイド
p = tf.sigmoid(f)

t = tf.placeholder(tf.float32, [None, 1])
#誤差関数と最小化
loss = -tf.reduce_sum(t*tf.log(p) + (1-t)*tf.log(1-p))
train_step = tf.train.AdamOptimizer().minimize(loss)

#セッションの開始
"""
sess= tf.Session()
sess.run(tf.initialize_all_variables())
#勾配降下法
i=0
for _ in range(200):
  i+=1
  sess.run(train_step, feed_dict = {x:train_x, t:train_t})
  if i % 2000 == 0:
    loss_val, acc_val = sess.run(
      [loss,accuracy],feed_dict={x:train_x,t:train_t})
    print('Step: %d, Loss: %f, Accuracy: %f'
          % (i, loss_val, acc_val))
"""