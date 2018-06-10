#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Test.png");
    if(image.empty())
    {
        cout << "Empty input image!";
        return -1;
    }

    Mat grayImg;
    cvtColor(image, grayImg, COLOR_BGR2GRAY);

    int bins = 256;
    int nimages = 1;
    int channels[] = {0};
    Mat mask;
    int dims = 1;
    int histSize[] = { bins };
    float rangeGS[] = {0, 256};
    const float* ranges[] = { rangeGS };
    bool uniform = true;
    bool accumulate = false;
    Mat histogram;
    calcHist(&grayImg,
             nimages,
             channels,
             mask,
             histogram,
             dims,
             histSize,
             ranges,
             uniform,
             accumulate);

    double maxVal = 0;
    minMaxLoc(histogram,
              0,
              &maxVal,
              0,
              0);

    int gRows = 533; // height
    int gCols = 800; // width
    Scalar backGroundColor = Scalar(0, 255, 255); // yellow
    Scalar graphColor = Scalar(0, 0, 0); // black
    int thickness = 2;
    LineTypes lineType = LINE_AA;

    Mat theGraph(gRows, gCols, CV_8UC(3), backGroundColor);

    Point p1(0,0), p2(0,0);
    for(int i=0; i<bins; i++)
    {
        float value = histogram.at<float>(i,0);
        value = maxVal - value; // invert
        value = value / maxVal * theGraph.rows; // scale
        line(theGraph,
             p1,
             Point(p1.x,value),
             graphColor,
             thickness,
             lineType);
        p1.y = p2.y = value;
        p2.x = float(i+1) * float(theGraph.cols) / float(bins);
        line(theGraph,
             p1, p2,
             Scalar(0,0,0));
        p1.x = p2.x;
    }

    imshow("Image", grayImg);
    imshow("Histogram", theGraph);
    waitKey();

    return 0;
}
