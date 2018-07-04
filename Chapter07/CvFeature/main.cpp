#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;



int main()
{
    double matchThresh;

    // ***** Read images
    Mat object = imread("TestObject.png");
    Mat scene = imread("TestScene.png");

    // ***** Create the algorithm accordingly
    int algorithm = 1;
    Ptr<Feature2D> detector;
    switch(algorithm)
    {

    case 1:
        detector = BRISK::create();
        matchThresh = 350;
        break;

    case 2:
        detector = KAZE::create();
        matchThresh = 0.1;
        break;

    case 3:
        detector = AKAZE::create();
        matchThresh = 250;
        break;

    }

    // ***** Detect features
    vector<KeyPoint> objKPs, scnKPs;
    detector->detect(object, objKPs);
    detector->detect(scene, scnKPs);

    // ***** Extract descriptors
    Mat objDesc, scnDesc;
    detector->compute(object, objKPs, objDesc);
    detector->compute(scene, scnKPs, scnDesc);

    // ***** Match descriptors
    Ptr<BFMatcher> matcher = BFMatcher::create();
    vector<DMatch> matches;
    matcher->match(objDesc, scnDesc, matches);

    // ***** Remove "bad" matches
    vector<DMatch> goodMatches;
    for(int i=0; i<objDesc.rows; i++)
    {
        if(matches[i].distance < matchThresh)
            goodMatches.push_back(matches[i]);
    }

    if(goodMatches.size() > 0)
    {
        cout << "Found " << goodMatches.size() << " good matches.";
    }
    else
    {
        cout << "Didn't find a single good match. Quitting!";
        return -1;
    }

    // ***** Draw matches
    Mat result;
    drawMatches(object,
                objKPs,
                scene,
                scnKPs,
                goodMatches,
                result,
                Scalar(0, 255, 0), // green for matched
                Scalar::all(-1), // unmatched color (default)
                vector<char>(), // empty mask
                DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // ***** Find the homography change between object and scene
    vector<Point2f> goodP1, goodP2;
    for(int i=0; i<goodMatches.size(); i++)
    {
        goodP1.push_back(objKPs[goodMatches[i].queryIdx].pt);
        goodP2.push_back(scnKPs[goodMatches[i].trainIdx].pt);
    }
    Mat homoChange = findHomography(goodP1, goodP2);

    // ***** Apply perpective change to find the result points
    vector<Point2f> corners1(4), corners2(4);
    corners1[0] = Point2f(0,0);
    corners1[1] = Point2f(object.cols-1, 0);
    corners1[2] = Point2f(object.cols-1, object.rows-1);
    corners1[3] = Point2f(0, object.rows-1);
    perspectiveTransform(corners1, corners2, homoChange);

    // ***** Correct the coordinates to fir the drawMatches result image
    for(int i=0; i<4; i++)
        corners2[i].x += object.cols;

    // ***** Draw the bounding lines
    line(result, corners2[0], corners2[1], Scalar::all(255), 2);
    line(result, corners2[1], corners2[2], Scalar::all(255), 2);
    line(result, corners2[2], corners2[3], Scalar::all(255), 2);
    line(result, corners2[3], corners2[0], Scalar::all(255), 2);

    imshow("Result", result);
    waitKey();

    return 0;
}
