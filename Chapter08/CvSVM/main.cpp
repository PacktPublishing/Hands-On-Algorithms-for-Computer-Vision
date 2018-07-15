#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace ml;

int main()
{
    const int SAMPLE_COUNT = 8;
    Ptr<TrainData> data;

    float samplesA[SAMPLE_COUNT][2]
            = { {250, 50},
                {125, 100},
                {50, 50},
                {150, 150},
                {100, 250},
                {250, 250},
                {150, 50},
                {50, 250} };
    Mat samples(SAMPLE_COUNT, 2, CV_32F, samplesA);

    int responsesA[SAMPLE_COUNT]
            = {2, 2, 2, 2, 1, 2, 2, 1};
    Mat responses(SAMPLE_COUNT, 1, CV_32S, responsesA);

    SampleTypes layout = ROW_SAMPLE;
    data = TrainData::create(samples,
                             layout,
                             responses);

    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(
                TermCriteria(TermCriteria::MAX_ITER +
                             TermCriteria::EPS,
                             100,
                             1e-6));

    if(!svm->train(data))
    {
        cout << "training failed" << endl;
        return -1;
    }

    Mat image = Mat::zeros(300,
                           300,
                           CV_8UC3);
    Vec3b blue(255,0,0), green(0,255,0);
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            Mat_<float> sampleMat(1,2);
            sampleMat << j,i;
            float response = svm->predict(sampleMat);

            if (response == 1)
                image.at<Vec3b>(i,j)  = green;
            else if (response == 2)
                image.at<Vec3b>(i,j)  = blue;
        }
    }

    Vec3b black(0,0,0), white(255,255,255), color;
    for(int i=0; i<SAMPLE_COUNT; i++)
    {
        Point p(samplesA[i][0],
                samplesA[i][1]);
        if (responsesA[i] == 1)
            color = black;
        else if (responsesA[i] == 2)
            color = white;
        circle(image,
               p,
               5,
               color,
               CV_FILLED);
    }

    imshow("image", image);
    waitKey();

    return 0;
}
