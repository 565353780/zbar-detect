#include "Code_Finder.h"

Code_Finder::Code_Finder()
{

}

Code_Finder::~Code_Finder()
{

}

Mat Code_Finder::findCode(Mat &image)
{
    double scale_num = 2.0;

    Mat scaled_image;

    resize(image, scaled_image, Size(0, 0), 1.0 / scale_num, 1.0 / scale_num);

//    Mat image_blurred;
//    pyrMeanShiftFiltering(scaled_image, image_blurred, 10, 100);

    Mat image_gray;
    cvtColor(scaled_image, image_gray, COLOR_BGR2GRAY);

    Mat image_bin;
    threshold(image_gray, image_bin, 150, 255, THRESH_BINARY);

//    Mat image_bin_flip;
//    transpose(image_bin, image_bin_flip);

//    Mat image_canny;
//    Canny(image_bin, image_canny, 3, 9, 3);

//    vector<Point> mean_point_vector = getMeanPointsFromBinMat(image_canny);

//    Mat zeros(image.rows, image.cols, CV_8UC1, Scalar(0));

//    for(int i = 0; i < mean_point_vector.size(); ++i)
//    {
//        circle(image, scale_num * mean_point_vector[i], 3, Scalar(255), 2);
//        circle(image_canny, mean_point_vector[i], 3, Scalar(255), 2);
//    }

    //fit points and read black and white codes
    //how to trans points to a line

    imshow("source", scaled_image);
    imshow("test", image_bin);
    waitKey(0);

    return image_bin;
}

int Code_Finder::getDist(Point point_1, Point point_2)
{
    int dist = abs(point_1.x - point_2.x) + abs(point_1.y - point_2.y);

    return dist;
}

Point Code_Finder::getMeanPoint(vector<Point> point_set)
{
    //1 : position mean
    //2 : bbox mean
    int mode = 2;

    Point mean_point(0, 0);

    if(point_set.size() == 0)
    {
        cout << "Code_Finder::getMeanPoint : No point input." << endl;
        return mean_point;
    }

    if(mode == 1)
    {
        double x_sum = 0;
        double y_sum = 0;

        for(int i = 0; i < point_set.size(); ++i)
        {
            x_sum += point_set[i].x;
            y_sum += point_set[i].y;
        }

        x_sum /= 1.0 * point_set.size();
        y_sum /= 1.0 * point_set.size();

        mean_point.x = x_sum;
        mean_point.y = y_sum;

        return mean_point;
    }
    else if(mode == 2)
    {
        double x_min = point_set[0].x;
        double x_max = x_min;
        double y_min = point_set[0].y;
        double y_max = y_min;

        if(point_set.size() > 1)
        {
            for(int i = 1; i < point_set.size(); ++i)
            {
                if(point_set[i].x < x_min)
                {
                    x_min = point_set[i].x;
                }
                if(point_set[i].x > x_max)
                {
                    x_max = point_set[i].x;
                }
                if(point_set[i].y < y_min)
                {
                    y_min = point_set[i].y;
                }
                if(point_set[i].y > y_max)
                {
                    y_max = point_set[i].y;
                }
            }
        }

        mean_point.x = (x_min + x_max) / 2.0;
        mean_point.y = (y_min + y_max) / 2.0;

        return mean_point;
    }
}

vector<vector<Point>> Code_Finder::getPointCluster(vector<Point> point_vector)
{
    vector<vector<Point>> point_cluster_vector;

    if(point_vector.size() == 0)
    {
        cout << "Code_Finder::getPointCluster : No point input." << endl;
        return point_cluster_vector;
    }

    if(point_vector.size() == 1)
    {
        point_cluster_vector.emplace_back(point_vector);
        return point_cluster_vector;
    }

    int merged_point_num = 0;
    vector<bool> point_merged_vector;
    point_merged_vector.resize(point_vector.size());
    for(int i = 0; i < point_merged_vector.size(); ++i)
    {
        point_merged_vector[i] = false;
    }

    while(merged_point_num < point_vector.size())
    {
        for(int i = 0; i < point_vector.size(); ++i)
        {
            if(!point_merged_vector[i])
            {
                vector<Point> current_point_cluster;
                current_point_cluster.emplace_back(point_vector[i]);
                point_cluster_vector.emplace_back(current_point_cluster);
                point_merged_vector[i] = true;
                ++merged_point_num;

                int current_merge_id = point_cluster_vector.size() - 1;

                bool new_point_merged = true;
                while(new_point_merged)
                {
                    new_point_merged = false;

                    for(int j = 0; j < point_vector.size(); ++j)
                    {
                        if(!point_merged_vector[j])
                        {
                            for(int k = 0; k < point_cluster_vector[current_merge_id].size(); ++k)
                            {
                                if(getDist(point_vector[j], point_cluster_vector[current_merge_id][k]) < 3)
                                {
                                    point_cluster_vector[current_merge_id].emplace_back(point_vector[j]);
                                    point_merged_vector[j] = true;
                                    ++merged_point_num;

                                    new_point_merged = true;
                                    break;
                                }
                            }
                        }

                        if(new_point_merged)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }

    return point_cluster_vector;
}

vector<Point> Code_Finder::getMeanPointsFromBinMat(Mat &image)
{
    vector<Point> point_vector;

    for(int i = 0; i < image.cols; ++i)
    {
        for(int j = 0; j < image.rows; ++j)
        {
            if(image.at<uchar>(j, i) == 255)
            {
                point_vector.emplace_back(Point(i, j));
            }
        }
    }

    if(point_vector.size() == 0)
    {
        cout << "Code_Finder::findCode : No point found as bound." << endl;
        return point_vector;
    }

    vector<vector<Point>> point_cluster_vector = getPointCluster(point_vector);

    vector<vector<Point>> valid_point_cluster_vector;

    for(int i = 0; i < point_cluster_vector.size(); ++i)
    {
        if(point_cluster_vector[i].size() >= 20)
        {
            valid_point_cluster_vector.emplace_back(point_cluster_vector[i]);
        }
    }

    cout << "point cluster num : " << point_cluster_vector.size() << " , " << valid_point_cluster_vector.size() << endl;

    vector<Point> mean_point_vector;

    for(int i = 0; i < valid_point_cluster_vector.size(); ++i)
    {
        Point current_mean_point = getMeanPoint(valid_point_cluster_vector[i]);

        mean_point_vector.emplace_back(current_mean_point);
    }

    return mean_point_vector;
}
