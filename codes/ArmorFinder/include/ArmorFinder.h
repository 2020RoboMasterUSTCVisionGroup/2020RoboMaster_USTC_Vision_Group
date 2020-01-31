//
// Created by xinyang on 19-3-27.
//
#ifndef _ARMOR_FINDER_H_
#define _ARMOR_FINDER_H_

#include<opencv2/opencv.hpp>
#include<string>
#include <Eigen/Core>

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
    cv::Rect2d rect;
    LightBlobs light_blobs;
    uint8_t box_color;
    int id;
    ArmorBox(const cv::Rect &pos, const LightBlobs &blobs, uint8_t color):rect(pos),light_blobs(blobs),box_color(color){};

    // 获取装甲板中心点
    cv::Point2f getCenter() const {
        return cv::Point2f(
                rect.x + rect.width / 2,
                rect.y + rect.height / 2
        );
    }
    // 获取两个灯条中心点的间距
    double getBlobsDistance() const {
        if (light_blobs.size() == 2) {
            auto &x = light_blobs[0].rect.center;
            auto &y = light_blobs[1].rect.center;
            return sqrt((x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y));
        } else {
            return 0;
        }
    }
    // 判断两个灯条的角度差
    static bool angelJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);
    // 判断两个灯条的高度差
    static bool heightJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);
    // 判断两个灯条的间距
    static bool lengthJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);
     // 判断两个灯条的长度比
    static bool lengthRatioJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);
    /* 判断两个灯条的错位度，不知道英文是什么！！！ */
    static bool CuoWeiDuJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);
     // 判断装甲板方向
    static bool boxAngleJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j);
    //判断是否为一对灯条
    static bool isCoupleLight(const LightBlob &light_blob_i, const LightBlob &light_blob_j, uint8_t enemy_color);
    
    static bool isValidBolbpair(const cv::Rect2d rect_left , const cv::Rect2d rect_right);
    // double lengthDistanceRatio() const; // 获取灯条中心距和灯条长度的比值
    // double getBoxDistance() const; // 获取装甲板到摄像头的距离

    // bool operator<(const ArmorBox &box) const; // 装甲板优先级比较
};
typedef std::vector<ArmorBox> ArmorBoxes;

//此处为灯条颜色，对应通道分离可按照此处更改
#define BLOB_RED 1
#define BLOB_BLUE 0
/**
 * 此处函数可单独建立一个控制流程的类封装，
 * 将lightbolbs以及armorbox的对象也存放在该类中
 * */
bool findLightBolbsSJTU(Mat &input_img);
bool findLightBolbsCSDN(Mat &input_img);
void showLightBlobs(const cv::Mat &input_image,string windows_name,const LightBlobs &light_blobs);
bool matchArmorBoxes(const cv::Mat &src, const LightBlobs &light_blobs, ArmorBoxes &armor_boxes);
void showArmorBoxes(std::string windows_name, const cv::Mat &src, const ArmorBoxes &armor_boxes);
#endif _ARMOR_FINDER_H_ 
