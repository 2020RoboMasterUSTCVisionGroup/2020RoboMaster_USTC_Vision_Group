//
// Created by xinyang on 19-7-10.
//
#include <opencv2/highgui.hpp>
#include<string.h>
#include "armor_finder.h"
#include <Eigen/Core>
    // 判断两个灯条的角度差
    static bool angelJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
        float angle_i = light_blob_i.rect.size.width > light_blob_i.rect.size.height ? light_blob_i.rect.angle :
                        light_blob_i.rect.angle - 90;
        float angle_j = light_blob_j.rect.size.width > light_blob_j.rect.size.height ? light_blob_j.rect.angle :
                        light_blob_j.rect.angle - 90;
        return abs(angle_i - angle_j) < 20;
    }
    // 判断两个灯条的高度差
    static bool heightJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
        cv::Point2f centers = light_blob_i.rect.center - light_blob_j.rect.center;
        return abs(centers.y) < 30;
    }
    // 判断两个灯条的间距
    static bool lengthJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
        double side_length;
        cv::Point2f centers = light_blob_i.rect.center - light_blob_j.rect.center;
        side_length = sqrt(centers.ddot(centers));
        return (side_length / light_blob_i.length < 10 && side_length / light_blob_i.length > 0.5);
    }
    // 判断两个灯条的长度比
    static bool lengthRatioJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
        return (light_blob_i.length / light_blob_j.length < 2.5
                && light_blob_i.length / light_blob_j.length > 0.4);
    }

    /* 判断两个灯条的错位度，不知道英文是什么！！！ */
    static bool CuoWeiDuJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
        float angle_i = light_blob_i.rect.size.width > light_blob_i.rect.size.height ? light_blob_i.rect.angle :
                        light_blob_i.rect.angle - 90;
        float angle_j = light_blob_j.rect.size.width > light_blob_j.rect.size.height ? light_blob_j.rect.angle :
                        light_blob_j.rect.angle - 90;
        float angle = (angle_i + angle_j) / 2.0 / 180.0 * 3.14159265459;
        if (abs(angle_i - angle_j) > 90) {
            angle += 3.14159265459 / 2;
        }
        Eigen::Vector2f orientation(cos(angle), sin(angle));
        Eigen::Vector2f p2p(light_blob_j.rect.center.x - light_blob_i.rect.center.x,
                    light_blob_j.rect.center.y - light_blob_i.rect.center.y);
        return abs(orientation.dot(p2p)) < 25;
    }
    // 判断装甲板方向
    static bool boxAngleJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
        float angle_i = light_blob_i.rect.size.width > light_blob_i.rect.size.height ? light_blob_i.rect.angle :
                        light_blob_i.rect.angle - 90;
        float angle_j = light_blob_j.rect.size.width > light_blob_j.rect.size.height ? light_blob_j.rect.angle :
                        light_blob_j.rect.angle - 90;
        float angle = (angle_i + angle_j) / 2.0;
        if (abs(angle_i - angle_j) > 90) {
            angle += 90.0;
        }
        return (-120.0 < angle && angle < -60.0) || (60.0 < angle && angle < 120.0);
    }
    static bool isCoupleLight(const LightBlob &light_blob_i, const LightBlob &light_blob_j, uint8_t enemy_color) {
    return light_blob_i.blob_color == enemy_color &&
           light_blob_j.blob_color == enemy_color &&
           lengthRatioJudge(light_blob_i, light_blob_j) &&
           lengthJudge(light_blob_i, light_blob_j) &&
           //           heightJudge(light_blob_i, light_blob_j) &&
           angelJudge(light_blob_i, light_blob_j) &&
           boxAngleJudge(light_blob_i, light_blob_j) &&
           CuoWeiDuJudge(light_blob_i, light_blob_j);

    }
 bool matchArmorBoxes(const cv::Mat &src, const LightBlobs &light_blobs, ArmorBoxes &armor_boxes){
        armor_boxes.clear();
        for (int i = 0; i < light_blobs.size() - 1; ++i) {
            for (int j = i + 1; j < light_blobs.size(); ++j) {
                if (!isCoupleLight(light_blobs.at(i), light_blobs.at(j), BLOB_RED)) {
                    continue;
                }
                cv::Rect2d rect_left = light_blobs.at(static_cast<unsigned long>(i)).rect.boundingRect();
                cv::Rect2d rect_right = light_blobs.at(static_cast<unsigned long>(j)).rect.boundingRect();
                double min_x, min_y, max_x, max_y;
                min_x = fmin(rect_left.x, rect_right.x) - 4;
                max_x = fmax(rect_left.x + rect_left.width, rect_right.x + rect_right.width) + 4;
                min_y = fmin(rect_left.y, rect_right.y) - 0.5 * (rect_left.height + rect_right.height) / 2.0;
                max_y = fmax(rect_left.y + rect_left.height, rect_right.y + rect_right.height) +
                        0.5 * (rect_left.height + rect_right.height) / 2.0;
                if (min_x < 0 || max_x > src.cols || min_y < 0 || max_y > src.rows) {
                    continue;
                }
                if ((max_y + min_y) / 2 < 120) continue;
                if ((max_x - min_x) / (max_y - min_y) < 0.8) continue;
                LightBlobs pair_blobs = {light_blobs.at(i), light_blobs.at(j)};
                armor_boxes.emplace_back(
                        cv::Rect2d(min_x, min_y, max_x - min_x, max_y - min_y),
                        pair_blobs,
                        BLOB_RED
                );
            }
        }
    return !armor_boxes.empty();
    }
// 旋转矩形的长宽比
static double lw_rate(const cv::RotatedRect &rect) {
    return rect.size.height > rect.size.width ?
           rect.size.height / rect.size.width :
           rect.size.width / rect.size.height;
}
// 轮廓面积和其最小外接矩形面积之比
static double areaRatio(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect) {
    return cv::contourArea(contour) / rect.size.area();
}
// 判断轮廓是否为一个灯条
static bool isValidLightBlob(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect) {
    return (1.2 < lw_rate(rect) && lw_rate(rect) < 10) &&
           //           (rect.size.area() < 3000) &&
           ((rect.size.area() < 50 && areaRatio(contour, rect) > 0.4) ||
            (rect.size.area() >= 50 && areaRatio(contour, rect) > 0.6));
}
// 判断灯条颜色(此函数可以有性能优化).
static uint8_t get_blob_color(const cv::Mat &src, const cv::RotatedRect &blobPos) {
    auto region = blobPos.boundingRect();
    region.x -= fmax(3, region.width * 0.1);
    region.y -= fmax(3, region.height * 0.05);
    region.width += 2 * fmax(3, region.width * 0.1);
    region.height += 2 * fmax(3, region.height * 0.05);
    region &= cv::Rect(0, 0, src.cols, src.rows);
    cv::Mat roi = src(region);
    int red_cnt = 0, blue_cnt = 0;
    for (int row = 0; row < roi.rows; row++) {
        for (int col = 0; col < roi.cols; col++) {
            red_cnt += roi.at<cv::Vec3b>(row, col)[2];
            blue_cnt += roi.at<cv::Vec3b>(row, col)[0];
        }
    }
    if (red_cnt > blue_cnt) {
        return BLOB_RED;
    } else {
        return BLOB_BLUE;
    }
}
// 判断两个灯条区域是同一个灯条
static bool isSameBlob(LightBlob blob1, LightBlob blob2) {
    auto dist = blob1.rect.center - blob2.rect.center;
    return (dist.x * dist.x + dist.y * dist.y) < 9;
}
// 开闭运算
static void imagePreProcess(cv::Mat &src) {
    static cv::Mat kernel_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode);

    static cv::Mat kernel_dilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate);

    static cv::Mat kernel_dilate2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate2);

    static cv::Mat kernel_erode2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode2);
}
 void drawLightBlobs(cv::Mat &src, const LightBlobs &blobs){
        for (const auto &blob:blobs) {
            Scalar color(0,255,0);
            if (blob.blob_color == BLOB_RED)
                color = Scalar(0, 0, 255);
            else if (blob.blob_color == BLOB_BLUE)
                color = Scalar(255, 0, 0);
            cv::Point2f vertices[4];
            blob.rect.points(vertices);
            for (int j = 0; j < 4; j++) {
                cv::line(src, vertices[j], vertices[(j + 1) % 4], color, 2);
            }
        }
    }
    void showArmorBoxes(std::string windows_name, const cv::Mat &src, const ArmorBoxes &armor_boxes) {
        static Mat image2show;
        if (src.type() == CV_8UC1) {// 黑白图像
            cvtColor(src, image2show, COLOR_GRAY2RGB);
        } else if (src.type() == CV_8UC3) { //RGB 彩色
            image2show = src.clone();
        }

        for (auto &box:armor_boxes) {
            if(box.box_color == BLOB_BLUE) {
                rectangle(image2show, box.rect, Scalar(0, 255, 0), 1);
                drawLightBlobs(image2show, box.light_blobs);
            }else if(box.box_color == BLOB_RED){
                rectangle(image2show, box.rect, Scalar(0, 255, 0), 1);
                drawLightBlobs(image2show, box.light_blobs);
            }
        }
        imshow(windows_name, image2show);
    }
// 在给定图像上寻找所有可能的灯条
//int main ()
bool findLightBolbsV2(Mat &input_img)
{
    LightBlobs light_blobs;
    cv::Mat color_channel;
    cv::Mat src_bin_light, src_bin_dim;
    std::vector<cv::Mat> channels;       // 通道拆分
     //Mat input_img = imread("aa.png");
    cv::split(input_img, channels);               /************************/
     color_channel = channels[2];        /************************/
    int light_threshold;
     light_threshold = 200;
    cv::threshold(color_channel, src_bin_light, light_threshold, 255, CV_THRESH_BINARY); // 二值化对应通道
    if (src_bin_light.empty()) return false;
    imagePreProcess(src_bin_light);                                  // 开闭运算

    cv::threshold(color_channel, src_bin_dim, 140, 255, CV_THRESH_BINARY); // 二值化对应通道
    if (src_bin_dim.empty()) return false;
    imagePreProcess(src_bin_dim); 

    // imshow("ee",src_bin_light);
    // imshow("e",src_bin_dim);
    // waitKey(0);

// 使用两个不同的二值化阈值同时进行灯条提取，减少环境光照对二值化这个操作的影响。
// 同时剔除重复的灯条，剔除冗余计算，即对两次找出来的灯条取交集。
    std::vector<std::vector<cv::Point>> light_contours_light, light_contours_dim;
    LightBlobs light_blobs_light, light_blobs_dim;
    std::vector<cv::Vec4i> hierarchy_light, hierarchy_dim;
    cv::findContours(src_bin_light, light_contours_light, hierarchy_light, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::findContours(src_bin_dim, light_contours_dim, hierarchy_dim, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    for (int i = 0; i < light_contours_light.size(); i++) {
        if (hierarchy_light[i][2] == -1) {
            cv::RotatedRect rect = cv::minAreaRect(light_contours_light[i]);
            if (isValidLightBlob(light_contours_light[i], rect)) {
                light_blobs_light.emplace_back(
                        rect, areaRatio(light_contours_light[i], rect), get_blob_color(input_img, rect)
                );
            }
        }
    }
    for (int i = 0; i < light_contours_dim.size(); i++) {
        if (hierarchy_dim[i][2] == -1) {
            cv::RotatedRect rect = cv::minAreaRect(light_contours_dim[i]);
            if (isValidLightBlob(light_contours_dim[i], rect)) {
                light_blobs_dim.emplace_back(
                        rect, areaRatio(light_contours_dim[i], rect), get_blob_color(input_img, rect)
                );
            }
        }
    }
    vector<int> light_to_remove, dim_to_remove;

    for (int l = 0; l != light_blobs_light.size(); l++) {
        for (int d = 0; d != light_blobs_dim.size(); d++) {
            if (isSameBlob(light_blobs_light[l], light_blobs_dim[d])) {
                if (light_blobs_light[l].area_ratio > light_blobs_dim[d].area_ratio) {
                    dim_to_remove.emplace_back(d);
                } else {
                    light_to_remove.emplace_back(l);
                }
            }
        }
    }
    sort(light_to_remove.begin(), light_to_remove.end(), [](int a, int b) { return a > b; });
    sort(dim_to_remove.begin(), dim_to_remove.end(), [](int a, int b) { return a > b; });
    for (auto x : light_to_remove) {
        light_blobs_light.erase(light_blobs_light.begin() + x);
    }
    for (auto x : dim_to_remove) {
        light_blobs_dim.erase(light_blobs_dim.begin() + x);
    }
    for (const auto &light : light_blobs_light) {
        light_blobs.emplace_back(light); 
     
    }
    for (const auto &dim : light_blobs_dim) {
        light_blobs.emplace_back(dim);
    }

    //showLightBlobs(input_img,"lightbolbs",light_blobs); 
    ArmorBoxes boxes;
    matchArmorBoxes(input_img,light_blobs,boxes);
    showArmorBoxes("res",input_img,boxes);
    return  0;
    
}
void showLightBlobs(const cv::Mat &input_image,string windows_name,const LightBlobs &light_blobs) {
            static Mat image2show;
            if (input_image.type() == CV_8UC1) { // 黑白图像
                cvtColor(input_image, image2show, COLOR_GRAY2RGB);
            } else if (input_image.type() == CV_8UC3) { //RGB 彩色
                image2show = input_image.clone();
            }

            for (const auto &light_blob:light_blobs) {
                Scalar color(0, 255, 0);
                
                cv::Point2f vertices[4];
                light_blob.rect.points(vertices);
                for (int j = 0; j < 4; j++) {
                    cv::line(image2show, vertices[j], vertices[(j + 1) % 4], color, 2);
                }
            }
            imshow(windows_name, image2show);
            waitKey(10);
    }
