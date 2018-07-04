#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Test.png");
    Mat imgGray;
    cvtColor(image, imgGray, COLOR_BGR2GRAY);

    double threshold1 = 100.0;
    double threshold2 = 200.0;
    int apertureSize = 3;
    bool L2gradient = false;
    Mat edges;
    Canny(image,
          edges,
          threshold1,
          threshold2,
          apertureSize,
          L2gradient);

    vector<vector<Point> > contours;
    findContours(edges,
                 contours,
                 CV_RETR_TREE,
                 CV_CHAIN_APPROX_SIMPLE,
                 Point(0, 0));

    RNG rng(12345); // any random number

    Mat result(edges.size(), CV_8UC3, Scalar(0));
    int thickness = 2;
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(rng.uniform(0, 255),
                              rng.uniform(0,255),
                              rng.uniform(0,255) );

        drawContours(result,
                     contours,
                     i,
                     color,
                     thickness);
    }

    imshow("image", image);
    imshow("result", result);
    imshow("edges", edges);

    waitKey();

    return 0;
}
