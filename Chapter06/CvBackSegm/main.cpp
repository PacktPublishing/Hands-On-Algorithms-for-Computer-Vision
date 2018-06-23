#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cam(0);
    if(!cam.isOpened())
        return -1;

    int history = 500;
    double dist2Threshold = 400.0;
    bool detectShadows = true;
    Ptr<BackgroundSubtractorKNN> bgs =
            createBackgroundSubtractorKNN(history,
                                          dist2Threshold,
                                          detectShadows);

    while(true)
    {
        Mat frame;
        cam >> frame;
        if(frame.empty())
            break;

        Mat fgMask;
        bgs->apply(frame,
                   fgMask);

        medianBlur(fgMask,fgMask,3);

        Mat fg;
        bitwise_and(frame, frame, fg, fgMask);

        Mat bg;
        bgs->getBackgroundImage(bg);

        imshow("Video", frame);
        imshow("Background", bg);
        imshow("Foreground Mask", fgMask);
        imshow("Foreground Image", fg);

        int key = waitKey(10);
        if(key == 27) // escape key
            break;
    }

    cam.release();

    return 0;
}
