//
// Created by sun on 19-7-11.
//

#include "energy.h"

using namespace std;
using namespace cv;

//extern McuData mcu_data;


//----------------------------------------------------------------------------------------------------------------------
// 此函数用于清空各vector
// ---------------------------------------------------------------------------------------------------------------------
void Energy::clearAll() {
    //fans.clear();
    armors.clear();
    //flow_strip_fans.clear();
    //target_armors.clear();
    //flow_strips.clear();
}


//----------------------------------------------------------------------------------------------------------------------
// 此函数用于图像预处理
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initImage(cv::Mat &src) {
static Mat src1;
    if (src.type() == CV_8UC3){
    //    cvtColor(src, src, COLOR_BGR2GRAY);
        vector<Mat> channels;                 /*利用vector对象拆分*/
    split(src, channels);                     /*调用通道拆分函数*/
    { 
        src = channels[2];             /*将红色提出来，红色是第三个通道*/   
        src1 = channels[0];            /*将蓝色提出来，红色是第一个通道*/  
    }  
    addWeighted(src, 1, src1,-1, 0.0, src);     //将两张图片按比例合成一张图片
    imshow("去噪",src);
    waitKey(0);
    }
    //if (mcu_data.enemy_color == ENEMY_BLUE){
    //   threshold(src, src, energy_part_param_.RED_GRAY_THRESH, 255, THRESH_BINARY);
    //} else if(mcu_data.enemy_color == ENEMY_RED){
    threshold(src, src, energy_part_param_.BLUE_GRAY_THRESH, 255, THRESH_BINARY);
    //imshow("bin", src);
    //waitKey(0);
    //if (show_energy || show_process)waitKey(1);
}

