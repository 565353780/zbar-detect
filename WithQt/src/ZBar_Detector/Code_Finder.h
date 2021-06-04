#ifndef CODE_FINDER_H
#define CODE_FINDER_H

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <zbar.h>

#include <vector>
#include <string.h>

using namespace std;
using namespace cv;
using namespace zbar;

class Code_Finder
{
public:
    Code_Finder();
    ~Code_Finder();

    Mat findCode(Mat &image);

private:
    int getDist(Point point_1, Point point_2);

    Point getMeanPoint(vector<Point> point_set);

    vector<vector<Point>> getPointCluster(vector<Point> point_vector);

    vector<Point> getMeanPointsFromBinMat(Mat &image);
};

#endif // CODE_FINDER_H
