"""
画像関連
"""
import numpy as np
import cv2
def CreateWhite(cols,rows):
  image = np.zeros((cols,rows,3),np.uint8)
  for i in range(image.shape[0]):
    for j in range(image.shape[1]):
      image[i,j] = [255,255,255]
  return image

def ShowImage(img):
  cv2.imshow("image",img)
  cv2.waitKey(0)
  cv2.destroyAllWindows()
  
def DrawType(img):
  height,weight = img.shape[:2]
  #print(height,weight)
  #中央縦線
  cv2.line(img,(0,int(height/2)),(weight,int(height/2)),(0,0,0),1)
  #中央横線
  cv2.line(img,(int(weight/2),0),(int(weight/2),height),(0,0,0),1)
  cv2.circle(img,(int(weight/4),int(height/4)),5,(0,0,0),1)
  
def ShowNitch(Pop):
  cols = 1000
  rows = 1000
  img = CreateWhite(cols,rows)

  height,weight = img.shape[:2]
  #中央縦線
  cv2.line(img,(0,int(height/2)),(weight,int(height/2)),(0,0,0),1)
  #中央横線
  cv2.line(img,(int(weight/2),0),(int(weight/2),height),(0,0,0),1)
  fontface = cv2.FONT_HERSHEY_COMPLEX
  for i in Pop:
    x = i[0][0]*10
    y = i[0][1]*10
    cv2.putText(img,str(i[2]),(int(x),int(y)),fontface,1.0,(0,0,0),2)
  
  ShowImage(img)