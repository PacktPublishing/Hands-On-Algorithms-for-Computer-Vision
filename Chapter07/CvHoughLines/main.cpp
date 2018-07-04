#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Test.png");

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

    vector<Vec2f> lines;
    double rho = 1.0; // 1 pixel, r resolution
    double theta = CV_PI / 180.0; // 1 degree, theta resolution
    int threshold = 100; // minimum number of intersections to "detect" a line
    HoughLines(edges,
               lines,
               rho,
               theta,
               threshold);

    Scalar color(0,0,255);
    int thickness = 2;
    for(int i=0; i<lines.size(); i++)
    {
        float rho = lines.at(i)[0];
        float theta = lines.at(i)[1];
        Point pt1, pt2;
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a*rho;
        double y0 = b*rho;
        pt1.x = int(x0 + 1000*(-b));
        pt1.y = int(y0 + 1000*(a));
        pt2.x = int(x0 - 1000*(-b));
        pt2.y = int(y0 - 1000*(a));
        line( image, pt1, pt2, color, thickness);
    }

    imshow("image", image);

    waitKey();

    return 0;
}
