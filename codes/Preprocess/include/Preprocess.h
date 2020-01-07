#ifndef __PREPROCESS__H
#define __PREPROCESS__H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;
//图像预处理
class Preprocess
{
public:
    Preprocess();   //构造函数，
    ~Preprocess();  //默认析构函数

    void run(Mat &src);     //run函数，运行
private:
    void setParamInit();    //设置参数
    void clearWhiteLight(); //  消除日光影响


};
#endif