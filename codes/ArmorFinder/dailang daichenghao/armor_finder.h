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
#define BLOB_RED 1
#define BLOB_BLUE 0


void showLightBlobs(const cv::Mat &input_image,string windows_name,const LightBlobs &light_blobs);
#endif /* _ARMOR_FINDER_H_ */
