#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void getHistogram(const Mat &grayImg, Mat &histogram)
{
    int bins = 256;
    int nimages = 1;
    int channels[] = {0};
    Mat mask;
    int dims = 1;
    int histSize[] = { bins };
    float rangeGS[] = {0, 256};
    const float* ranges[] = { rangeGS };
    bool uniform = true;
    bool accumulate = false;
    calcHist(&grayImg,
             nimages,
             channels,
             mask,
             histogram,
             dims,
             histSize,
             ranges,
             uniform,
             accumulate);
}

int main()
{
    Mat image1 = imread("Test.png");
    Mat image2 = imread("Test2.png");
    if(image1.empty() || image2.empty())
    {
        cout << "Empty input image!";
        return -1;
    }

    Mat grayImg1, grayImg2, histogram1, histogram2;
    cvtColor(image1, grayImg1, COLOR_BGR2GRAY);
    cvtColor(image2, grayImg2, COLOR_BGR2GRAY);

    getHistogram(grayImg2, histogram1);
    getHistogram(grayImg1, histogram2);

    cout << "HISTCMP_CORREL: " << compareHist(histogram1, histogram2, HISTCMP_CORREL) << endl;
    cout << "HISTCMP_CHISQR: " << compareHist(histogram1, histogram2, HISTCMP_CHISQR) << endl;
    cout << "HISTCMP_INTERSECT: " << compareHist(histogram1, histogram2, HISTCMP_INTERSECT) << endl;
    cout << "HISTCMP_BHATTACHARYYA: " << compareHist(histogram1, histogram2, HISTCMP_BHATTACHARYYA) << endl;
    cout << "HISTCMP_HELLINGER: " << compareHist(histogram1, histogram2, HISTCMP_HELLINGER) << endl;
    cout << "HISTCMP_CHISQR_ALT: " << compareHist(histogram1, histogram2, HISTCMP_CHISQR_ALT) << endl;
    cout << "HISTCMP_KL_DIV: " << compareHist(histogram1, histogram2, HISTCMP_KL_DIV) << endl;

    return 0;
}
