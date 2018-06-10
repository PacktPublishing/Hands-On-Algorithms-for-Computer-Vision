#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Test2.png");
    if(image.empty())
    {
        cout << "Empty input image!";
        return -1;
    }

    Mat imgHsv, hue;
    vector<Mat> hsvChannels;
    cvtColor(image, imgHsv, COLOR_BGR2HSV);
    split(imgHsv, hsvChannels);
    hue = hsvChannels[0];

    int bins = 36;
    int histSize[] = {bins};
    int nimages = 1;
    int dims = 1;
    int channels[] = {0};
    float rangeHue[] = {0, 180};
    const float* ranges[] = {rangeHue};
    bool uniform = true;
    bool accumulate = false;
    Mat histogram, mask;

    calcHist(&hue,
             nimages,
             channels,
             mask,
             histogram,
             dims,
             histSize,
             ranges,
             uniform,
             accumulate);

    double maxVal;
    minMaxLoc(histogram,
              0,
              &maxVal,
              0,
              0);

    int gW = 800, gH = 100;
    Mat theGraph(gH, gW, CV_8UC3, Scalar::all(0));

    Mat colors(1, bins, CV_8UC3);
    for(int i=0; i<bins; i++)
    {
        colors.at<Vec3b>(i) =
                Vec3b(saturate_cast<uchar>(
                          (i+1)*180.0/bins), 255, 255);
    }
    cvtColor(colors, colors, COLOR_HSV2BGR);
    Point p1(0,0), p2(0,theGraph.rows-1);
    for(int i=0; i<bins; i++)
    {
        float value = histogram.at<float>(i,0);
        value = maxVal - value; // invert
        value = value / maxVal * theGraph.rows; // scale
        p1.y = value;
        p2.x = float(i+1) * float(theGraph.cols) / float(bins);
        rectangle(theGraph,
                  p1,
                  p2,
                  Scalar(colors.at<Vec3b>(i)),
                  CV_FILLED);
        p1.x = p2.x;
    }

    imshow("Image", image);
    imshow("Graph", theGraph);

    waitKey();

    return 0;
}
