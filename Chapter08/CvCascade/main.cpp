#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace ml;

int main()
{
    CascadeClassifier faceDetector;
    if(!faceDetector.load("haarcascade_frontalface_default.xml"))
    {
        cout << "Can't find cascade classifier." << endl;
        return -1;
    }

    VideoCapture cam;
    if(!cam.open(0))
    {
        cout << "Can't open camera." << endl;
        return -1;
    }

    Scalar red = Scalar(0,0,255);
    Scalar green = Scalar(0,255,0);
    int thickness = 2;
    double scale = 0.75;

    while(true)
    {
        Mat frame;
        cam >> frame;
        if(frame.empty())
            break;

        vector<Rect> faces;

        double t = (double)getTickCount();

        faceDetector.detectMultiScale(frame,
                                      faces);

        t = ((double)getTickCount() - t)/getTickFrequency();
        t *= 1000; // convert to ms

        for(int i=0; i<faces.size(); i++)
        {
            rectangle(frame,
                      faces[i],
                      red,
                      thickness);
        }

        putText(frame,
                "Took " + to_string(int(t)) + "ms to detect",
                Point(0, frame.rows-1),
                FONT_HERSHEY_SIMPLEX,
                scale,
                green,
                thickness);

        imshow("Camera", frame);
        if(waitKey(10) == 27) // escape key
            break;

    }

    return 0;
}
