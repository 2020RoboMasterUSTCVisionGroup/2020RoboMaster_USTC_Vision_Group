//
// Created by xinyang on 19-3-27.
//
#include<opencv2/opencv.hpp>
#include<string.h>
#ifndef _ARMOR_FINDER_H_
#define _ARMOR_FINDER_H_
using namespace std;
using  namespace cv;
/******************* 灯条类定义 ***********************/
class LightBlob {
public:
    cv::RotatedRect rect;   //灯条位置
    double area_ratio;
    double length;          //灯条长度
    uint8_t blob_color;      //灯条颜色

    LightBlob(cv::RotatedRect &r, double ratio, uint8_t color) : rect(r), area_ratio(ratio), blob_color(color) {
        length = max(rect.size.height, rect.size.width);
    };
    double max(double x,double y){
        return x>y?x:y;
    }
    LightBlob() = default;
};
typedef std::vector<LightBlob> LightBlobs;
class ArmorBox{
public:
    typedef enum{
        FRONT, SIDE, UNKNOWN
    } BoxOrientation;

    cv::Rect2d rect;
    LightBlobs light_blobs;
    uint8_t box_color;
    int id;

   // explicit ArmorBox(const cv::Rect &pos=cv::Rect2d(), const LightBlobs &blobs=LightBlobs(), uint8_t color=0, int i=0);

    ArmorBox(const cv::Rect &pos, const LightBlobs &blobs, uint8_t color):rect(pos),light_blobs(blobs),box_color(color){};

    cv::Point2f getCenter() const; // 获取装甲板中心
    double getBlobsDistance() const; // 获取两个灯条中心间距
    double lengthDistanceRatio() const; // 获取灯条中心距和灯条长度的比值
    double getBoxDistance() const; // 获取装甲板到摄像头的距离
    BoxOrientation getOrientation() const; // 获取装甲板朝向(误差较大，已弃用)

    bool operator<(const ArmorBox &box) const; // 装甲板优先级比较
};
typedef std::vector<ArmorBox> ArmorBoxes;
#define BLOB_RED 1
#define BLOB_BLUE 0


void showLightBlobs(const cv::Mat &input_image,string windows_name,const LightBlobs &light_blobs);
#endif /* _ARMOR_FINDER_H_ */
