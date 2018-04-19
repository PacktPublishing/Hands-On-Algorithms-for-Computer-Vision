#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("D:/dev/git/Hands-On-Algorithms-for-Computer-Vision/Test.png");
    if(!image.empty())
    {
        imshow("image", image);
        waitKey();
    }
    else
    {
        cout << "Empty image!" << endl;
    }

    return 0;
}
