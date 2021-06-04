#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    Mat image = imread("/home/chli/public/test_zbar/8.jpg");

    vector<decodedObject> decodedObject_vector;

    ZBar_Detector zbar_detector;

    zbar_detector.decode(image, decodedObject_vector);

    for(int i = 0; i < decodedObject_vector.size(); ++i)
    {
        cout << "Code " << i << " :" << endl;
        cout << "type : " << decodedObject_vector[i].type << endl;
        cout << "data : " << decodedObject_vector[i].data << endl;
        cout << "location : " << endl << decodedObject_vector[i].location << endl << endl;
    }

    return;

    zbar_detector.display(image, decodedObject_vector);
}
