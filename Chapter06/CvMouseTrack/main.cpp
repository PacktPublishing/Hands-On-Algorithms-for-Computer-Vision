#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Point objectPos;

void onMouse(int, int x, int y, int, void*)
{
    objectPos.x = x;
    objectPos.y = y;
}

int main()
{
    KalmanFilter kalman(4,2);

    Mat_<float> tm(4, 4); // transition matrix
    tm << 1,0,1,0, // next x = 1x+0y+1x'+0y'
            0,1,0,1, // next x'= 0x+1y+0x'+1y'
            0,0,1,0, // next y = 0x+0y+1x'+0y
            0,0,0,1; // next y'= 0x+0y+0x'+1y'
    kalman.transitionMatrix =  tm;
    Mat_<float> pos(2,1);
    pos.at<float>(0) = 0;
    pos.at<float>(1) = 0;

    kalman.statePre.at<float>(0) = 0; // init x
    kalman.statePre.at<float>(1) = 0; // init y
    kalman.statePre.at<float>(2) = 0; // init x'
    kalman.statePre.at<float>(3) = 0; // init y'

    setIdentity(kalman.measurementMatrix);

    setIdentity(kalman.processNoiseCov,
                Scalar::all(0.000001));

    string window = "Canvas";
    namedWindow(window);
    setMouseCallback(window, onMouse);

    vector<Point> trackRoute;
    vector<Point> mouseRoute;

    while(waitKey(10) < 0)
    {
        // empty canvas
        Mat canvas(500, 1000, CV_8UC3, Scalar(255, 255, 255));

        pos(0) = objectPos.x;
        pos(1) = objectPos.y;

        Mat estimation = kalman.correct(pos);

        Point estPt(estimation.at<float>(0),
                    estimation.at<float>(1));

        trackRoute.push_back(estPt);
        if(trackRoute.size() > 100)
            trackRoute.erase(trackRoute.begin());

        polylines(canvas,
                  trackRoute,
                  false,
                  Scalar(0,0,255),
                  5);

        mouseRoute.push_back(objectPos);
        if(mouseRoute.size() > 100)
            mouseRoute.erase(mouseRoute.begin());
        polylines(canvas,
                  mouseRoute,
                  false,
                  Scalar(0,0,0),
                  2);

        imshow(window, canvas);

        kalman.predict();
    }
    return 0;

}
