import cv2
import numpy as np
import matplotlib.pyplot as plt

nome=input("digite o nome da imagem: ")
img = cv2.imread(nome, cv2.IMREAD_GRAYSCALE)

ksize = 0
while ( ksize < 3 or ksize >31 ):
    ksize=int(input("Digite o tamanho do kernel do Sobel: "))
img_sobel=cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize)

nome_out=input("digite o nome da imagem de saida: ")
cv2.imwrite(nome_out,  img_sobel)
#print("X = ", img.cols(), "Y = ", img.rows(), "Colors depth = ", img.depth())
cv2.imshow('Original',img)
cv2.imshow('Sobel',img_sobel)
cv2.waitKey(0)
cv2.destroyAllWindows()
