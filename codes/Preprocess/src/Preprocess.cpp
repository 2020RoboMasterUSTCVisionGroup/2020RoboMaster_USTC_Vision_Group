/*-----------------------------文件--------------------
*   文件名：Preprocess.cpp
*   作者：  孙霖
*   功能：  载入图像预处理，目前实现对日光滤出
------------------------------------------------------*/

#include "Preprocess.h"

char EnemyColor='r';
void Preprocess::run(cv::Mat &src)
{
    clearWhiteLight(src);
    cvtColor(src, src, COLOR_GRAY2RGB);     //将黑白图像转换成三通道
}

void Preprocess::clearWhiteLight(Mat &src)
{
    Mat src_blue,src_red;       //红蓝通道图像
    if (src.type() == CV_8UC3){
        vector<Mat> channels;                 /*利用vector对象拆分*/
    split(src, channels);                     /*调用通道拆分函数*/
    { 
        src_red = channels[2];             /*将红色提出来，红色是第三个通道*/   
        src_blue = channels[0];            /*将蓝色提出来，红色是第一个通道*/  
    } 
        if(EnemyColor=='r')
        addWeighted(src_red, 1, src_blue,-1, 0.0, src);     //将两张图片按比例合成一张图片
        else 
        addWeighted(src_red, -1, src_blue,1, 0.0, src);     //蓝减红        
        
    }
}