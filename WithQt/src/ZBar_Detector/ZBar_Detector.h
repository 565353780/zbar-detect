#ifndef ZBAR_DETECTOR_H
#define ZBAR_DETECTOR_H

#include "ZBar_Detector/Code_Finder.h"

struct decodedObject
{
    string type;
    string data;
    vector<Point> location;
};

class ZBar_Detector
{
public:
    ZBar_Detector();
    ~ZBar_Detector();

    void decode(Mat &image, vector<decodedObject> &decodedObject_vector);

    void display(Mat &image, vector<decodedObject> &decodedObject_vector);

private:
    Code_Finder code_finder_;
};

#endif // ZBAR_DETECTOR_H
