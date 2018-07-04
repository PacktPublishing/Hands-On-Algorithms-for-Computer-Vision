#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat object = imread("TestObject.png");
    Mat objectGr;
    cvtColor(object, objectGr, COLOR_BGR2GRAY);
    Mat scene = imread("TestScene.png");
    Mat sceneGr;
    cvtColor(scene, sceneGr, COLOR_BGR2GRAY);

    TemplateMatchModes method = TM_CCOEFF_NORMED;

    Mat result;
    matchTemplate(sceneGr, objectGr, result, method);

    if(method % 2 == 0)
        normalize(result, result, 0.0, 1.0, NORM_MINMAX, -1);

    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    Rect rect(maxLoc.x,
              maxLoc.y,
              object.cols,
              object.rows);

    Scalar color(0, 0, 255);
    int thickness = 2;
    rectangle(scene,
              rect,
              color,
              thickness);

    imshow("object", object);
    imshow("scene", scene);
    imshow("result", result);

    waitKey();

    return 0;
}
