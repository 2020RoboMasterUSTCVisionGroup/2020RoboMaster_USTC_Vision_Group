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
    double lengthDistanceRatio() const; // 获取灯条中心距和灯条长度的比值
    double getBoxDistance() const; // 获取装甲板到摄像头的距离

    bool operator<(const ArmorBox &box) const; // 装甲板优先级比较
};
typedef std::vector<ArmorBox> ArmorBoxes;

#define BLOB_RED 1
#define BLOB_BLUE 0

bool findLightBolbsSJTU(Mat &input_img);
bool findLightBolbsCSDN(Mat &input_img);
void showLightBlobs(const cv::Mat &input_image,string windows_name,const LightBlobs &light_blobs);
bool matchArmorBoxes(const cv::Mat &src, const LightBlobs &light_blobs, ArmorBoxes &armor_boxes);
void showArmorBoxes(std::string windows_name, const cv::Mat &src, const ArmorBoxes &armor_boxes);
#endif /* _ARMOR_FINDER_H_ */
