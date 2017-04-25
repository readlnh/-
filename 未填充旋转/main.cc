#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

const double pi = acos(-1);

int main(int argc, char **argv) {
    Mat src = imread("./lena.jpg");
    namedWindow("show", CV_WINDOW_AUTOSIZE);
    imshow("show", src);
    Mat dst = Mat::zeros(src.size(), src.type());
    double theta = pi / 3;
    cout << sin(theta) << endl;
    int x0 = src.rows / 2;
    int y0 = src.cols / 2;
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            int x = (i - x0) * cos(-theta) - (j - y0) * sin(-theta) + x0;
            int y = (i - x0) * sin(-theta) + (j - y0) * cos(-theta) + y0;
            if(x >= 0 && x < src.rows && y >= 0 && y < src.cols) 
                dst.at<Vec3b>(x,y) = src.at<Vec3b> (i, j);
        }
    }
    namedWindow("xx", CV_WINDOW_AUTOSIZE);
    imshow("xx", dst);
    imwrite("2.jpg", dst);
    waitKey(0);
    return 0;
}
