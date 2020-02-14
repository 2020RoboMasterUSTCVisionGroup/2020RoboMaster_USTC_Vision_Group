#include <stdio.h>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main()
{
    Mat srcImage = imread("1.jpeg");
    imshow("1",srcImage);
    waitKey(0);
}