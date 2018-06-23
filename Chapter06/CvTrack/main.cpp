#include "opencv2/opencv.hpp"
#include <Windows.h>

using namespace std;
using namespace cv;

TermCriteria criteria(TermCriteria::MAX_ITER
                      + TermCriteria::EPS,
                      20,
                      1.0);

bool selecting = false;
Rect selection;
Point spo; // selection point origin

void onMouse(int event, int x, int y, int, void*)
{
    switch(event)
    {
    case EVENT_LBUTTONDOWN:
    {
        spo.x = x;
        spo.y = y;
        selection.x = spo.x;
        selection.y = spo.y;
        selection.width = 0;
        selection.height = 0;
        selecting = true;

    } break;
    case EVENT_LBUTTONUP:
    {
        selecting = false;
    } break;
    default:
    {
        selection.x = min(x, spo.x);
        selection.y = min(y, spo.y);
        selection.width = abs(x - spo.x);
        selection.height = abs(y - spo.y);
    } break;
    }
}

int main()
{
    VideoCapture cam(0);
    if(!cam.isOpened())
        return -1;

    Rect srchWnd;

    string outputWindow = "Display";
    namedWindow(outputWindow);
    cv::setMouseCallback(outputWindow, onMouse);

    vector<Point> route;
    Mat histogram, backProject, mask;

    int key = -1;
    while(key != ' ')
    {
        Mat frame;
        cam >> frame;
        if(frame.empty())
            break;

        Mat frmHsv, hue;
        vector<Mat> hsvChannels;
        cvtColor(frame, frmHsv, COLOR_BGR2HSV);
        split(frmHsv, hsvChannels);
        hue = hsvChannels[0];

        int bins = 120;
        int nimages = 1;
        int channels[] = {0};
        float rangeHue[] = {0, 180};
        const float* ranges[] = {rangeHue};
        int histSize[] = { bins };
        bool uniform = true;

        if(selecting && selection.area() > 0)
        {
            Mat sel(frame, selection);

            int lbHue = 00 , hbHue = 180;
            int lbSat = 30 , hbSat = 256;
            int lbVal = 30 , hbVal = 230;

            inRange(frmHsv,
                    Scalar(lbHue,lbSat,lbVal),
                    Scalar(hbHue, hbSat, hbVal),
                    mask);

            Mat roi(hue, selection);
            Mat maskroi(mask, selection);

            calcHist(&roi,
                     nimages,
                     channels,
                     maskroi,
                     histogram,
                     1,
                     histSize,
                     ranges,
                     uniform);

            normalize(histogram,
                      histogram, 0, 255, NORM_MINMAX);

            bitwise_not(sel, sel);

            srchWnd = selection;
        }
        else if(!histogram.empty())
        {

            double scale = 1.0;
            calcBackProject(&hue,
                            nimages,
                            channels,
                            histogram,
                            backProject,
                            ranges,
                            scale,
                            uniform);

            erode(backProject,
                  backProject,
                  Mat());


            //            meanShift(backProject,
            //                      srchWnd,
            //                      criteria);

            CamShift(backProject,
                     srchWnd,
                     criteria);

            Point p(srchWnd.x + srchWnd.width/2,
                    srchWnd.y + srchWnd.height/2);

            route.push_back(p);
            if(route.size() > 60)
                route.erase(route.begin());

            rectangle(frame,
                      srchWnd,
                      Scalar(0,0,255),
                      2);

            polylines(frame,
                      route,false,
                      Scalar(0,255,0),
                      2);

            cvtColor(backProject, backProject, COLOR_GRAY2BGR);
            rectangle(backProject,
                      srchWnd,
                      Scalar(0,0,255),
                      2);

            polylines(backProject,
                      route,false,
                      Scalar(0,255,0),
                      2);

        }

        switch(key)
        {
        case 'b':
            if(!backProject.empty()) imshow(outputWindow, backProject);
            else imshow(outputWindow, frame);
            break;
        case 'v': default: imshow(outputWindow, frame);
            break;

        }

        int k = waitKey(10);
        if(k > 0)
            key = k;
    }

    cam.release();

    return 0;

}
