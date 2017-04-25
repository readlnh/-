#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

int main(int argc, char **argv) {
    Mat src = imread("./2.jpg");
    namedWindow("xxx", CV_WINDOW_AUTOSIZE);
    imshow("xxx", src);
    double xRadio = 3;
    double yRadio = 3;
    int rows = (int)(src.rows * xRadio + 0.5);
    int cols = (int)(src.cols * yRadio + 0.5);
    Size dsize = Size(rows, cols);
    Mat dst = Mat::zeros(dsize, src.type());
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            double row = i / xRadio;
            double col = j / yRadio;
            int lRow = (int)(row);
            int nRow = lRow + 1;
            int lCol = (int)(col);
            int rCol = lCol + 1;
            double u = row - lRow;
            double v = col - lCol;
            if((row >= src.rows - 1) && (col >= src.cols - 1)) {
                dst.at<Vec3b>(i, j) = src.at<Vec3b>(lRow, lCol);
            }
            else if(row >= src.rows - 1) {
                dst.at<Vec3b>(i, j) = v * src.at<Vec3b>(lRow, lCol) 
                + (1 - v) * src.at<Vec3b>(lRow, rCol);
            }
            else if(col >= src.cols - 1) {
                dst.at<Vec3b>(i, j) = u * src.at<Vec3b>(lRow, lCol) 
                + (1 - u) * src.at<Vec3b>(nRow, lCol);
            }
            else {
                Vec3b f1 = v * src.at<Vec3b>(lRow, lCol) + (1 - v) * src.at<Vec3b>(lRow, rCol);
                Vec3b f2 = v * src.at<Vec3b>(nRow, lCol) + (1 - v) * src.at<Vec3b>(nRow, rCol);
                dst.at<Vec3b>(i, j) = u * f1 + (1 - u) * f2;
            }
        }
    }
    namedWindow("x", CV_WINDOW_AUTOSIZE);
    imshow("x", dst);
	imwrite("双线性插值法1.jpg", dst); 
    waitKey(0);
    return 0;
}
