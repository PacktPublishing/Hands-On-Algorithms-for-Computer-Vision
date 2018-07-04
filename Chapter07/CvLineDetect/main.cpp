#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Test.png");
    Mat imgGray;
    cvtColor(image, imgGray, COLOR_BGR2GRAY);

    Ptr<LineSegmentDetector> detector = createLineSegmentDetector();

    vector<Vec4f> lines;
    detector->detect(imgGray,
                     lines);

    Mat result(image.size(),
               CV_8UC3,
               Scalar(0, 0, 0));

    //    detector->drawSegments(result,
    //                           lines);

    // or the following

    Scalar color(0,0,255);
    int thickness = 2;
    for(int i=0; i<lines.size(); i++)
    {
        line(result,
             Point(lines.at(i)[0],
                lines.at(i)[1]),
             Point(lines.at(i)[2],
                lines.at(i)[3]),
             color,
             thickness);
    }

    imshow("image", image);
    imshow("result", result);

    waitKey();

    return 0;
}
