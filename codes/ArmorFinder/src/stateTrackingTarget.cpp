
/*-----------------------------文件--------------------
*   文件名：LightBox.cpp
*   作者：  王妍璐 江培玲
*   功能：  对图片进行通道拆分和腐蚀膨胀以提取出灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------
#include "ArmorFinder.h"
#include "cans.h"
using namespace cv;
using namespace std;

extern Can can;
static void sendPosition(cv::Rect2d &rect,Mat src){
    unsigned char data[8];
    // double relative_x=box.getCenter().x-640;
    // double relative_y=box.getCenter().y-512;
    data[0]=(int(rect.x + rect.width / 2)>>8)&0xFF;
    data[1]=(int(rect.x + rect.width / 2))&0xFF;
    data[2]=(int(rect.y + rect.width / 2)>>8)&0xFF;
    data[3]=(int(rect.y + rect.width / 2))&0xFF;
    data[4]=((int((src.cols)/2))>>8)&0xFF;
    data[5]=(int((src.cols)/2))&0xFF;
    data[6]=((int((src.rows)/2))>>8)&0xFF;
    data[7]=(int((src.rows)/2))&0xFF;
    can.canTansfer(data);
    // cout<<"x"<<relative_x<<",y:"<<relative_y<<endl;
}
bool AutoAiming::stateTrackingTarget(cv::Mat &srcImage,cv::Mat &processImage) 
{
    
    //位置目标
    auto pos = target_box.rect;
    if(!tracker->update(srcImage, pos)){ // 使用KCFTracker进行追踪
        //追踪失败
        target_box = ArmorBox();
        cout<<"Track fail!"<<endl;
        return false;
    }
    cv::circle(srcImage,Point(pos.x + pos.width / 2,pos.y + pos.height / 2),
                5,cv::Scalar(0,0,255),2);

    Point predict=kf->predict(Point(pos.x + pos.width / 2,pos.y + pos.height / 2));
    //舍弃前五帧预测
    if(tracking_cnt>5){
        cv::circle(srcImage,predict,5,cv::Scalar(0,255,255),2);
        // sendPosition(pos,srcImage);
    }    

    showArmorBox("tracker", srcImage, pos);
    //追踪出界
    if((pos & cv::Rect2d(0, 0, 1920, 1200)) != pos){
        target_box = ArmorBox();
        //LOGW("Track out range!");
        cout<<"Track out range!"<<endl;
        return false;
    }
    //cout<<"box的大小位置x:"<<target_box.rect.x<<" y:"<<target_box.rect.y<<endl;   

    // 获取相较于追踪区域两倍长款的区域，用于重新搜索，获取灯条信息
    cv::Rect2d bigger_rect;
    bigger_rect.x = pos.x - pos.width / 2.0;
    bigger_rect.y = pos.y - pos.height / 2.0;
    bigger_rect.height = pos.height * 2;
    bigger_rect.width  = pos.width * 2;
    bigger_rect &= cv::Rect2d(0, 0, 1920, 1080);
    //cout<<"bigger的大小位置x:"<<bigger_rect.x<<" y:"<<bigger_rect.y<<endl;
    //cout<<"picture的大小x:"<<srcImage.rows<<" y:"<<srcImage.cols<<endl;
    cv::Mat roi_src = srcImage(bigger_rect).clone();
    //cout<<"追踪更新成功"<<tracking_cnt<<endl;
    cv::Mat roi_process = processImage(bigger_rect).clone();
    ArmorBox box;
    // 在区域内重新搜索。
    //cout<<"box的大小位置x:"<<box.rect.x<<" y:"<<box.rect.y<<endl;
    if(findArmorBoxTop(roi_src,roi_process,box)) 
    { // 如果成功获取目标，则利用搜索区域重新更新追踪器
        target_box = box;
        target_box.rect.x += bigger_rect.x; //　添加roi偏移量
        target_box.rect.y += bigger_rect.y;
        for(auto &blob : target_box.light_blobs){
            blob.rect.center.x += bigger_rect.x;
            blob.rect.center.y += bigger_rect.y;
        }
        tracker = TrackerKCF::create();
        tracker->init(srcImage, target_box.rect);
        return true;
    }
    else
    {    // 如果没有成功搜索目标，则使用判断是否跟丢。
        //cout<<"没有成功搜索目标"<<endl;
        roi_process = processImage(pos).clone();
         //　分类器不可用，使用常规方法判断
        cv::Mat roi_gray;
        cv::cvtColor(roi_process, roi_gray, CV_RGB2GRAY);
        cv::threshold(roi_gray, roi_gray, 180, 255, cv::THRESH_BINARY);
        contour_area = cv::countNonZero(roi_gray);
        if(abs(cv::countNonZero(roi_gray) - contour_area) > contour_area * 0.3){
            target_box = ArmorBox();
            return false;
        }
        target_box.rect = pos;
        target_box.light_blobs.clear();
        return false;
    }
    return true;
    // namedWindow("ROI",0);
    // resizeWindow("ROI",600,400);
    // imshow("ROI", roi);
    // waitKey(0);
}