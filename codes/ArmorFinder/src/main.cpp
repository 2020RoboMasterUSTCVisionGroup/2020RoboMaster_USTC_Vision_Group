#include "opencv2/opencv.hpp"
using namespace cv;
int lightBox(Mat image);
int main(){
   lightBox(imread("./test.png"));
   waitKey(0);
}
