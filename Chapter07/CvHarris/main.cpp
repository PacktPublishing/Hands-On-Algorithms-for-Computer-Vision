#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("Test.png");
    cvtColor(image, image, COLOR_BGR2GRAY);

    Mat result;
    int blockSize = 2;
    int ksize = 3;
    double k = 1.0;
    BorderTypes borderType = BORDER_DEFAULT;
    cornerHarris(image,
                 result,
                 blockSize,
                 ksize,
                 k,
                 borderType);

    normalize(result, result, 0.0, 1.0, NORM_MINMAX, -1);

    imshow("image", image);
    imshow("result", result);

    waitKey();

    return 0;
}
