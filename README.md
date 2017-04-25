数字图像专题报告 1405014339 徐凌霄


#1.图像缩放

图像的缩放主要用于改变图像的大小，缩放后图像的图像的宽度和高度会发生变化。水平缩放系数，控制图像宽度的缩放，其值为1，则图像的宽度不变；垂直缩放系数控制图像高度的缩放，其值为1，则图像的高度不变。如果水平缩放系数和垂直缩放系数不相等，那么缩放后图像的宽度和高度的比例会发生变化，会使图像变形。要保持图像宽度和高度的比例不发生变化，就需要水平缩放系数和垂直缩放系数相等。

##1.1 缩放原理

设水平缩放系数为sx，垂直缩放系数为sy，(x0,y0)为缩放前坐标，(x,y)为缩放后坐标，其缩放的坐标映射关系：
 $\begin{equation}
  \left\{
   \begin{aligned}
   x = x0 \times sx \\
   y = y0 \times sy \\
   \end{aligned}
   \right.
  \end{equation}$

矩阵表示的形式为：
$\begin{bmatrix}
 x& y & 1 
\end{bmatrix}
= \begin{bmatrix}
x0 & y0 & 1
\end{bmatrix}
\begin{bmatrix}
 sx& 0 & 0 \\ 
 0& sy & 0\\ 
 0& 0 & 1
\end{bmatrix}
$

这是向前映射，在缩放的过程改变了图像的大小，使用向前映射会出现映射重叠和映射不完全的问题，所以这里更关心的是向后映射，也就是输出图像通过向后映射关系找到其在原图像中对应的像素。

向后映射关系：
 $\begin{equation}
  \left\{
   \begin{aligned}
   x0 = \frac{x}{sx} \\
   y0 = \frac{y}{sy} \\
   \end{aligned}
   \right.
  \end{equation}$

##1.2基于OpenCV的缩放实现

在图像缩放的时首先需要计算缩放后图像的大小，设newWidth，newHeight为缩放后的图像的宽和高，width,height为原图像的宽度和高度，那么有：

 $\begin{equation}
  \left\{
   \begin{aligned}
   newWidth = s_x \times width\\
  newHeight = s_y \times height\\
   \end{aligned}
   \right.
  \end{equation}$

然后遍历缩放后的图像，根据向后映射关系计算出缩放的像素在原图像中像素的位置，如果得到的浮点坐标，就需要使用插值算法取得近似的像素值。

根据上面公式可知，缩放后图像的宽和高用原图像宽和高和缩放因子相乘即可。

```
int rows = (int)(src.rows * xRadio + 0.5);
int cols = (int)(src.cols * yRadio + 0.5);
```

在向后映射时有可能得到浮点坐标，这里使用最邻近插值和双线性插值来处理。

###最邻近插值
```
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
```


最邻近插值只需要对浮点坐标“四舍五入”运算。但是在四舍五入的时候有可能使得到的结果超过原图像的边界（只会比边界大1），所以要进行下修正。

###双线性插值

双线性插值的精度要比最邻近插值好很多，相对的其计算量也要大的多。双线性插值使用浮点坐标周围四个像素的值按照一定的比例混合近似得到浮点坐标的像素值。

设浮点坐标F，其周围的四个整数坐标别为T1,T2,T3,T4，并且F和其左上角的整数坐标的纵轴差的绝对值为n，横轴差的绝对值为n。有下面公式计算得到：
 $\begin{equation}
  \left\{
   \begin{aligned}
   F_1 = m * T_1 + (1 - m) * T_2  \\
  F_2 = m * T_3 + (1 - m) * T_4  \\
  F = n * F_1 + (1 - n) * F_2  \\
   \end{aligned}
   \right.
  \end{equation}$



在实现的时候首先要根据浮点坐标计算出其周围的四个整数坐标
```
 double row = i / xRadio;
 double col = j / yRadio;
 int lRow = (int)(row);
 int nRow = lRow + 1;
 int lCol = (int)(col);
 int rCol = lCol + 1;
 double u = row - lRow;
 double v = col - lCol;
```

缩放放后图像的坐标(i,j),根绝向后映射关系找到其在原图像中对应的坐标(i / xRatio,j / yRatio)，接着找到改坐标周围的四个整数坐标(lcol,lRow),(lCol,nrow),

(rCol,lRow),(rCo1,nRow)。下面根据双线性插值公式得到浮点坐标的像素值
```
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
```

由于使用四个像素进行计算，在边界的时候，会有不存在的像素，这里把在图像的右下角、最后一行、最后一列三种特殊情形分别处理。
#2.图像旋转
##2.1旋转原理

图像的旋转就是让图像按照某一点旋转指定的角度。图像旋转后不会变形，但是其垂直对称抽和水平对称轴都会发生改变，旋转后图像的坐标和原图像坐标之间的关系已不能通过简单的加减乘法得到，而需要通过一系列的复杂运算。而且图像在旋转后其宽度和高度都会发生变化，其坐标原点会发生变化。

图像所用的坐标系不是常用的笛卡尔，其左上角是其坐标原点，X轴沿着水平方向向右，Y轴沿着竖直方向向下。而在旋转的过程一般使用旋转中心为坐标原点的笛卡尔坐标系，所以图像旋转的第一步就是坐标系的变换。设旋转中心为(x0,y0)，(x’,y’)是旋转后的坐标，(x,y)是旋转后的坐标，则坐标变换如下：

$\begin{equation}
  \left\{
   \begin{aligned}
   x'  = x - x_0 \\
   y' = -y + y_0 \\
   \end{aligned}
   \right.
  \end{equation}$


矩阵表示为：

$\begin{bmatrix}
 x'& y' & 1 
\end{bmatrix}
= \begin{bmatrix}
x & y & 1
\end{bmatrix}
\begin{bmatrix}
 1& 0 & 0 \\ 
 0& -1 & 0\\ 
 -x_0& y_0 & 1
\end{bmatrix}
$
在最终的实现中，常用到的是有缩放后的图像通过映射关系找到其坐标在原图像中的相应位置，这就需要上述映射的逆变换

$\begin{bmatrix}
 x& y & 1 
\end{bmatrix}
= \begin{bmatrix}
x' & y' & 1
\end{bmatrix}
\begin{bmatrix}
 1& 0 & 0 \\ 
 0& -1 & 0\\ 
 -x_0& y_0 & 1
\end{bmatrix}
$

坐标系变换到以旋转中心为原点后，接下来就要对图像的坐标进行变换。

image

上图所示，将坐标(x0,y0)顺时针方向旋转a,得到(x1,y1)。

旋转前有：
$\begin{equation}
  \left\{
   \begin{aligned}
   x_0 = r \cos b \\
   y_0 = r \sin b \\
   \end{aligned}
   \right.
  \end{equation}$


旋转a后有：
$\begin{equation}
  \left\{
   \begin{aligned}
   x_1 = x_0 \cos b + y_0 \sin a\\
   y_1 = -x_0 \sin b + y_0 \cos a\\
   \end{aligned}
   \right.
  \end{equation}$

矩阵的表示形式：
$\begin{bmatrix}
 x1& y1 & 1 
\end{bmatrix}
= \begin{bmatrix}
x0 & y0 & 1
\end{bmatrix}
\begin{bmatrix}
 \cos a& - \sin a & 0 \\ 
 \sin a & \cos a & 0\\ 
 0& 0 & 1
\end{bmatrix}$

其逆变换：
$\begin{bmatrix}
 x0& y0 & 1 
\end{bmatrix}
= \begin{bmatrix}
x1 & y1 & 1
\end{bmatrix}
\begin{bmatrix}
 \cos a& - \sin a & 0 \\ 
 \sin a & \cos a & 0\\ 
 0& 0 & 1
\end{bmatrix}$

由于在旋转的时候是以旋转中心为坐标原点的，旋转结束后还需要将坐标原点移到图像左上角，也就是还要进行一次变换。这里需要注意的是，旋转中心的坐标(x0,y0)实在以原图像的左上角为坐标原点的坐标系中得到，而在旋转后由于图像的宽和高发生了变化，也就导致了旋转后图像的坐标原点和旋转前的发生了变换。

这里需要注意的一点，旋转后由于得到的坐标是浮点数，我们并不一定能保证每一个像素都有值，所以可能会产生‘空穴’，这时我们可以采用最近邻插值或双线性插值来填充。

为填充旋转
```
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
```
填充旋转
```
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
            int x = (i - x0) * cos(theta) - (j - y0) * sin(theta) + x0;
            int y = (i - x0) * sin(theta) + (j - y0) * cos(theta) + y0;
            
			if(x >= 0 && x < src.rows && y >= 0 && y < src.cols) 
                dst.at<Vec3b>(i,j) = src.at<Vec3b> (x, y);
        }
    }
    namedWindow("xx", CV_WINDOW_AUTOSIZE);
    imshow("xx", dst);
    imwrite("2.jpg", dst);
    waitKey(0);
    return 0;
}
```

#结语
在旋转和放大中，我们采用的算法还是比较粗糙的，实际上利用三线性甚至多线性插值效果应会有比较大的提升，同时也可用图像增强等技术来使图像得到更好的效果。
