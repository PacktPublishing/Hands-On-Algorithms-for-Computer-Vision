#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Test.png");
    Mat imgGray;
    cvtColor(image, imgGray, COLOR_BGR2GRAY);

    vector<Point2f> corners;
    int maxCorners = 500;
    double qualityLevel = 0.01;
    double minDistance = 10;
    Mat mask;
    int blockSize = 3;
    int gradientSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;
    goodFeaturesToTrack(imgGray,
                        corners,
                        maxCorners,
                        qualityLevel,
                        minDistance,
                        mask,
                        blockSize,
                        gradientSize,
                        useHarrisDetector,
                        k);

    Scalar color(0, 0, 255);
    MarkerTypes markerType = MARKER_TILTED_CROSS;
    int markerSize = 8;
    int thickness = 2;
    for(int i=0; i<corners.size(); i++)
    {
        drawMarker(image,
                   corners[i],
                   color,
                   markerType,
                   markerSize,
                   thickness);
    }

    imshow("image", image);

    waitKey();

    return 0;
}
