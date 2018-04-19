#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    cout << cv::getBuildInformation();
    return 0;
}
