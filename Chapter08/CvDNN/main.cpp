#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace ml;
using namespace dnn;

/*
    download all labels from here
    http://amin-ahmadi.com/downloadfiles/qt-opencv-tensorflow/class-names.txt
*/

const vector<string> labels = { "person", "bicycle" , "car"};

int main()
{
    Net network = readNetFromTensorflow("frozen_inference_graph.pb",
                                        "frozen_inference_graph.pbtxt");
    if(network.empty())
    {
        cout << "Can't read TensorFlow model." << endl;
        return -1;
    }

    VideoCapture cam;
    if(!cam.open(0))
    {
        cout << "Can't open camera." << endl;
        return -1;
    }

    Scalar green = Scalar(0,255,0);
    Scalar white = Scalar(255,255,255);
    Scalar red = Scalar(0,0,255);
    int thickness = 2;
    double scale = 0.75;

    while(true)
    {
        Mat frame;
        cam >> frame;
        if(frame.empty())
            break;

        const int inWidth = 300;
        const int inHeight = 300;
        const float meanVal = 127.5; // 255 divided by 2
        const float inScaleFactor = 1.0f / meanVal;
        bool swapRB = true;
        bool crop = false;
        Mat inputBlob = blobFromImage(frame,
                                      inScaleFactor,
                                      Size(inWidth, inHeight),
                                      Scalar(meanVal, meanVal, meanVal),
                                      swapRB,
                                      crop);

        network.setInput(inputBlob);

        double t = (double)getTickCount();

        Mat result = network.forward();

        t = ((double)getTickCount() - t)/getTickFrequency();
        t *= 1000; // convert to ms

        Mat detections(result.size[2], result.size[3], CV_32F, result.ptr<float>());

        const float confidenceThreshold = 0.5f;
        for(int i=0; i<detections.rows; i++)
        {
            float confidence = detections.at<float>(i, 2);
            if(confidence > confidenceThreshold)
            {
                int objectClass = (int)(detections.at<float>(i, 1))
                        - 1;
                int left = static_cast<int>(
                            detections.at<float>(i, 3) * frame.cols);
                int top = static_cast<int>(
                            detections.at<float>(i, 4) * frame.rows);
                int right = static_cast<int>(
                            detections.at<float>(i, 5) * frame.cols);
                int bottom = static_cast<int>(
                            detections.at<float>(i, 6) * frame.rows);
                rectangle(frame, Point(left, top),
                          Point(right, bottom), Scalar(0, 255, 0));
                String label = "ID = " + to_string(objectClass);
                if(objectClass < labels.size())
                    label = labels[objectClass];

                int baseLine = 0;
                Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX,
                                             0.5, 2, &baseLine);
                top = max(top, labelSize.height);

                rectangle(frame,
                          Point(left, top - labelSize.height),
                          Point(left + labelSize.width, top + baseLine),
                          white,
                          CV_FILLED);

                putText(frame, label, Point(left, top),
                        FONT_HERSHEY_SIMPLEX, 0.5, red);
            }
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
