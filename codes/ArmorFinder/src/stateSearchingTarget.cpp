
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
extern bool show_armor_boxes;
extern bool show_light_blobs;

void sendPosition_search(cv::Rect2d &rect,Mat src){
    unsigned char data[8];
    // double relative_x=box.getCenter().x-640;
    // double relative_y=box.getCenter().y-512;
    //cout<<"到这"<<endl;
    data[0]=(int(rect.x + rect.width / 2)>>8)&0xFF;
    data[1]=(int(rect.x + rect.width / 2))&0xFF;
    data[2]=(int(rect.y + rect.height / 2)>>8)&0xFF;
    data[3]=(int(rect.y + rect.height / 2))&0xFF;
    data[4]=((int((src.cols)/2))>>8)&0xFF;
    data[5]=(int((src.cols)/2))&0xFF;
    data[6]=((int((src.rows)/2))>>8)&0xFF;
    data[7]=(int((src.rows)/2))&0xFF;
    can.canTansfer(data);
    //cout<<"Send Messege"<<endl;
    // cout<<"x"<<relative_x<<",y:"<<relative_y<<endl;
}

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
    sendPosition_search(box.rect, srcImage);
    //sendPosition(box.rect,g_srcImage);
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