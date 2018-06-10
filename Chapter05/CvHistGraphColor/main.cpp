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

    Mat imgChannels[3];
    Mat histograms[3];
    split(image, imgChannels);

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

    int gRows = 533; // height
    int gCols = 800; // width
    Scalar backGroundColor = Scalar(0, 0, 0);
    int thickness = 3;
    LineTypes lineType = LINE_AA;

    Mat theGraph(gRows, gCols, CV_8UC(3), backGroundColor);

    for(int i=0; i<3; i++)
    {
        calcHist(&imgChannels[i],
                 nimages,
                 channels,
                 mask,
                 histograms[i],
                 dims,
                 histSize,
                 ranges,
                 uniform,
                 accumulate);

        double maxVal = 0;
        minMaxLoc(histograms[i],
                  0,
                  &maxVal,
                  0,
                  0);


        Scalar graphColor = Scalar(i == 0 ? 255 : 0,
                                   i == 1 ? 255 : 0,
                                   i == 2 ? 255 : 0);

        Point p1(0,0), p2(0,0);
        for(int j=0; j<bins; j++)
        {
            float value = histograms[i].at<float>(j,0);
            value = maxVal - value; // invert
            value = value / maxVal * theGraph.rows; // scale
            line(theGraph,
                 p1,
                 Point(p1.x,value),
                 graphColor,
                 thickness,
                 lineType);
            p1.y = p2.y = value;
            p2.x = float(j+1) * float(theGraph.cols) / float(bins);
            line(theGraph,
                 p1, p2,
                 Scalar(0,0,0));
            p1.x = p2.x;
        }
    }

    imshow("Image", image);
    imshow("Histogram", theGraph);

    waitKey();

    return 0;
}
