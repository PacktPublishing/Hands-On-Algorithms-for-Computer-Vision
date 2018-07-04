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

    imshow("image", image);
    imshow("edges", edges);

    waitKey();

    return 0;
}
