#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string window = "Image";
string trackbar = "Radius";

Mat image = imread("Test.png");
Point center(image.cols/2, image.rows/2);
int radius = 25;
Scalar color = Scalar(0, 255, 0);
int thickness = 2;
LineTypes lineType = LINE_AA;
int shift = 0;

void drawCircle(int, void*)
{
    Mat temp = image.clone();

    circle(temp,
           center,
           radius,
           color,
           thickness,
           lineType,
           shift);

    imshow(window, temp);
}

int main()
{    
    namedWindow(window);

    createTrackbar(trackbar,
                   window,
                   &radius,
                   min(image.rows, image.cols) / 2,
                   drawCircle);

    setTrackbarMin(trackbar, window, 25);
    setTrackbarMax(trackbar, window, min(image.rows, image.cols) / 2);

    drawCircle(0,0);
    waitKey();

    return 0;
}
