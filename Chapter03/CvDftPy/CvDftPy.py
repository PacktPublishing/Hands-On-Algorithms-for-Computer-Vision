# To be able to omit cv2 in code, use the following import statement (not recommended)
#from cv2 import *

# Otherwise use the following
import cv2
import numpy

image = cv2.imread("Test.png", cv2.IMREAD_GRAYSCALE)
if image is not None:
    # calculate optimal size for DFT calculation
    optRows = cv2.getOptimalDFTSize( image.shape[0] )
    optCols = cv2.getOptimalDFTSize( image.shape[1] )

    # resize and add zero (black) pixels
    resizedImg = cv2.copyMakeBorder(image,
                        0,
                        optRows - image.shape[0],
                        0,
                        optCols - image.shape[1],
                        cv2.BORDER_CONSTANT,
                        0,
                        0)

    # create two channels, one containing the resize image
    # the other containing zeros in a matrix with same size
    channels = [ numpy.mat(resizedImg, numpy.float) , numpy.zeros(resizedImg.shape, numpy.float) ]
    
    # Construct a single image using 2 channels
    complexImg = cv2.merge( channels )

    # perform DFT and save the result in the same place
    cv2.dft(complexImg, complexImg)

    # Split the result to its channels again,
    # since in the complex matrix result,
    # first channel contains real parts and,
    # second channel contains imaginary parts.
    cv2.split(complexImg, channels)

    mag = cv2.magnitude(channels[0], channels[1])

    # switch the magnitudes to logarithmic scale
    mag += 1
    cv2.log(mag, mag)

    # rearrange the quadrants
    cx = int(mag.shape[1] / 2)
    cy = int(mag.shape[0] / 2)

    mag_rows, mag_cols = mag.shape
    # crop the spectrum, if it has an odd number of rows or columns
    mag = mag[0:(mag_rows & -2), 0:(mag_cols & -2)]
    cx = int(mag_rows/2)
    cy = int(mag_cols/2)

    Q1 = mag[0:cx, 0:cy]
    Q2 = mag[cx:cx+cx, 0:cy]
    Q3 = mag[0:cx, cy:cy+cy]
    Q4 = mag[cx:cx+cx, cy:cy+cy]

    tmp = numpy.copy(Q1)
    mag[0:cx, 0:cy] = Q4
    mag[cx:cx + cx, cy:cy + cy] = tmp

    tmp = numpy.copy(Q2)
    mag[cx:cx + cx, 0:cy] = Q3
    mag[0:cx, cy:cy + cy] = tmp

    

    # normalize to the displayable range of values
    cv2.normalize(mag, mag, 0.0, 1.0, cv2.NORM_MINMAX)

    if mag is not None:
        cv2.imshow("image", image)
        cv2.imshow("output", mag)
        cv2.waitKey()

else:
    print("Empty image!")
