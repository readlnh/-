#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

int main(int argc, char **argv) {
    Mat src = imread("./2.jpg");
    namedWindow("原图", CV_WINDOW_AUTOSIZE);
    imshow("原图", src);
    double xRadio = 3;
    double yRadio = 3;
    int rows = (int)(src.rows * xRadio + 0.5);
    int cols = (int)(src.cols * yRadio + 0.5);
    Size dsize = Size(rows, cols);
    //Mat dst = Mat(dsize, CV_8U);
    Mat dst = Mat::zeros(dsize, src.type());
    for(int i = 0; i < rows; i++) {
        int row = (int)(i / xRadio + 0.5);
        if(row >= src.rows) row--;
        for(int j = 0; j < cols; j++) {
            int col = (int)(j / yRadio + 0.5); 
            if(col >= src.cols) col--;
            dst.at<Vec3b>(i,j) = src.at<Vec3b>(row, col);         
        }
    }
    namedWindow("放大后", CV_WINDOW_AUTOSIZE);
    imshow("放大后", dst);
	imwrite("最邻近插值法1.jpg",dst);
    waitKey(0);
    return 0;
}
