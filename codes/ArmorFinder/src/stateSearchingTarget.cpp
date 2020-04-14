
/*-----------------------------文件--------------------
*   文件名：LightBox.cpp
*   作者：  王妍璐 江培玲
*   功能：  对图片进行通道拆分和腐蚀膨胀以提取出灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------
#include "ArmorFinder.h"
using namespace cv;
using namespace std;
//参数配置,key
bool show_armor_box = false;
bool show_armor_boxes = false;
bool show_light_blobs = false;
bool show_origin = false;
bool run_with_camera = false;
bool save_video = false;
bool wait_uart = false;
bool save_labelled_boxes = false;
bool show_process = false;
bool show_energy = false;
bool save_mark = false;
bool show_info = false;
bool run_by_frame = false;

bool AutoAiming::findArmorBoxTop(cv::Mat &srcImage,cv::Mat &processImage,ArmorBox &box)
{
    LightBlobs light_blobs;
    box.rect = cv::Rect2d(0,0,0,0);
    box.id=-1;
    //寻找所有的灯条
    if(!findLightBolbsSJTU(srcImage,processImage,light_blobs))
    {
        return false;
    }
 
    //显示所有的灯条
    if (show_light_blobs && state==SEARCHING_STATE) 
    {
       drawLightBlobs(srcImage,light_blobs);
    }

    // 对灯条进行匹配得出装甲板候选区
    if(!matchArmorBoxes(processImage,light_blobs,armor_boxes))
    {
        return false;
    }
    //显示所有装甲板
    if (show_armor_boxes && state==SEARCHING_STATE) 
    {
        showArmorBoxes("boxes", srcImage, armor_boxes);
    }
    //choose the first armorbox as the target box
    box = armor_boxes[0];
    if (box.rect == cv::Rect2d(0, 0, 0, 0)) 
    {
        return false;
    }
    return true;
}
/**
*@author：王妍璐 江培玲
*@name：main()
*@return:void
*@function：
*@para：box: img:
*其他要注意的地方
**/  
bool AutoAiming::stateSearchingTarget(cv::Mat &g_srcImage,cv::Mat &g_processImage)
{
    if(findArmorBoxTop(g_srcImage,g_processImage,target_box))
    {
        return true;
    }
    else
    {
        return false;
    }
    
        
}