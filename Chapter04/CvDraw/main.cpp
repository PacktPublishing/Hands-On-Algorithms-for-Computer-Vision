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

    // ***************************** Arrowed line *****************************
    Point pt1(25, image.rows/2);
    Point pt2(image.cols/2 - 25, image.rows/2);
    Scalar color = Scalar(0,255,0);
    int thickness = 5;
    int lineType = LINE_AA;
    int shift = 0;
    double tipLength = 0.2;

    arrowedLine(image,
                pt1,
                pt2,
                color,
                thickness,
                lineType,
                shift,
                tipLength);

    // ***************************** Circle *****************************
    Point center(image.cols/2,
                 image.rows/2);
    int radius = 200;
    circle(image,
           center,
           radius,
           color,
           thickness,
           lineType,
           shift);

    // ***************************** Rectangle *****************************
    Rect rect(pt1,pt2);
    rectangle(image,
              rect,
              color,
              thickness,
              lineType,
              shift);

    // ***************************** Ellipse *****************************
    Size axes(200, 100);
    double angle = 20.0;
    double startAngle = 0.0;
    double endAngle = 360.0;
    ellipse(image,
            center,
            axes,
            angle,
            startAngle,
            endAngle,
            color,
            thickness,
            lineType,
            shift);

    // ***************************** Polyline *****************************
    vector<Point> pts;
    pts.push_back(Point(100, 100));
    pts.push_back(Point(50, 150));
    pts.push_back(Point(50, 200));
    pts.push_back(Point(150, 200));
    pts.push_back(Point(150, 150));
    bool isClosed = true;
    polylines(image,
              pts,
              isClosed,
              color,
              thickness,
              lineType,
              shift);

    imshow("Image", image);
    waitKey();

    return 0;
}
