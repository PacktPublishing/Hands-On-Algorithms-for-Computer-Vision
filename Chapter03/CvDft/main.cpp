#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{

    // read image grayscale
    Mat image = imread("Test.png", IMREAD_GRAYSCALE);
    if(image.empty())
    {
        cout << "Epmty image!";
        return -1; // error
    }

    // calculate optimal size for DFT calculation
    int optRows = getOptimalDFTSize( image.rows );
    int optCols = getOptimalDFTSize( image.cols );

    // resize and add zero (black) pixels
    Mat resizedImg;
    copyMakeBorder(image,
                   resizedImg,
                   0,
                   optRows - image.rows,
                   0,
                   optCols - image.cols,
                   BORDER_CONSTANT,
                   Scalar::all(0));

    /* create two channels, one containing the resize image
     * the other containing zeros in a matrix with same size */
    vector<Mat> channels = {Mat_<float>(resizedImg),
                            Mat::zeros(resizedImg.size(), CV_32F)};

    // Construct a single image using 2 channels
    Mat complexImage;
    merge(channels, complexImage);

    // perform DFT and save the result in the same place
    dft(complexImage, complexImage);

    /* Split the result to its channels again,
     * since in the complex matrix result,
     * first channel contains real parts and,
     * second channel contains imaginary parts. */
    split(complexImage, channels);

    Mat mag;
    magnitude(channels[0], channels[1], mag);

    // switch the magnitudes to logarithmic scale
    mag += Scalar::all(1);
    log(mag, mag);

    // rearrange the quadrants
    int cx = mag.cols/2;
    int cy = mag.rows/2;

    Mat Q1(mag, Rect(0, 0, cx, cy));
    Mat Q2(mag, Rect(cx, 0, cx, cy));
    Mat Q3(mag, Rect(0, cy, cx, cy));
    Mat Q4(mag, Rect(cx, cy, cx, cy));

    Mat tmp;
    Q1.copyTo(tmp);
    Q4.copyTo(Q1);
    tmp.copyTo(Q4);

    Q2.copyTo(tmp);
    Q3.copyTo(Q2);
    tmp.copyTo(Q3);

    // normalize to the displayable range of values
    normalize(mag, mag, 0.0, 1.0, CV_MINMAX);

    if(!image.empty() && !mag.empty())
    {
        imshow("image", image);
        imshow("output", mag);
        waitKey();
    }
    else
    {
        cout << "Empty images detected!" << endl;
    }

    return 0;
}
