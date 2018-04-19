# To be able to omit cv2 in code, use the following import statement (not recommended)
#from cv2 import *

# Otherwise use the following
import cv2

image = cv2.imread("d:/onedrive/me.jpg") #, cv2.IMREAD_GRAYSCALE)
if image is not None :
    cv2.imshow("image", image)
    cv2.waitKey()

else:
    print("Empty image!")