# To be able to omit cv2 in code, use the following import statement (not recommended)
#from cv2 import *

# Otherwise use the following
import cv2
import numpy

image = cv2.imread("Test.png")
if image is not None :

    lut = numpy.zeros((1, 256), numpy.uint8)
    for i in range(0, 255):
        if i < 125:
            lut[0, i] = 0
        elif i > 175:
            lut[0, i] = 255
        else :
            lut[0, i] = i

    result = cv2.LUT(image, lut)

    cv2.imshow("image", result)
    cv2.waitKey()

else:
    print("Empty image!")