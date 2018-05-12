#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    // read image grayscale
    Mat image = imread("Test.png");

    if(image.empty())
    {
        cout << "Empty input image!";
        return -1;
    }

    Mat lut(1, 256, CV_8UC1);
    for(int i=0; i<256; i++)
    {
        if(i < 125)
            lut.at<uchar>(0, i) = 0;
        else if(i > 175)
            lut.at<uchar>(0, i) = 255;
        else
            lut.at<uchar>(0, i) = i;
    }

    Mat result;
    LUT(image, lut, result);

    if(!image.empty() || !result.empty())
    {
        imshow("image", image);
        imshow("output", result);
        waitKey();
    }
    else
    {
        cout << "Empty images detected!" << endl;
    }

    return 0;
}
