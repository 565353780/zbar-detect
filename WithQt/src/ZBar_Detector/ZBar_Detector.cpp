#include "ZBar_Detector.h"

ZBar_Detector::ZBar_Detector()
{

}

ZBar_Detector::~ZBar_Detector()
{

}

void ZBar_Detector::decode(Mat &image, vector<decodedObject> &decodedObject_vector)
{
    ImageScanner scanner;

    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

//    Mat imGray;
//    cvtColor(image, imGray, COLOR_BGR2GRAY);

//    Image zbar_image(image.cols, image.rows, "Y800", (uchar *)imGray.data, image.cols * image.rows);

    Mat code = code_finder_.findCode(image);

//    return;

    Image zbar_image(code.cols, code.rows, "Y800", (uchar *)code.data, code.cols * code.rows);

    int n = scanner.scan(zbar_image);

    for(Image::SymbolIterator symbol = zbar_image.symbol_begin(); symbol != zbar_image.symbol_end(); ++symbol)
    {
        decodedObject obj;

        obj.type = symbol->get_type_name();
        obj.data = symbol->get_data();

//        cout << "type : " << obj.type << endl;
//        cout << "data : " << obj.data << endl;

        for(int i = 0; i < symbol->get_location_size(); ++i)
        {
            obj.location.emplace_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
        }

        decodedObject_vector.emplace_back(obj);
    }
}

void ZBar_Detector::display(Mat &image, vector<decodedObject> &decodedObject_vector)
{
    for(int i = 0; i < decodedObject_vector.size(); ++i)
    {
        vector<Point> points = decodedObject_vector[i].location;
        vector<Point> hull;

        if(points.size() > 4)
        {
            convexHull(points, hull);
        }
        else
        {
            hull = points;
        }

        int n = hull.size();

        for(int j = 0; j < n; ++j)
        {
            line(image, hull[j], hull[(j + 1) % n], Scalar(255, 0, 0), 3);
        }
    }

    imshow("ZBar Detect Result", image);
    waitKey(0);
}
