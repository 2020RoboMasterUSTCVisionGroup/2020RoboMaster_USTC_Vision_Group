#ifndef ENERGY_H
#define ENERGY_H

#include<opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <stdio.h>
#include <time.h>

#define ENEMY_BLUE 0
#define ENEMY_RED 1

using std::vector;
using namespace cv;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------
// 此结构体包括能量机关参数
// ---------------------------------------------------------------------------------------------------------------------
struct EnergyPartParam {
	int RED_GRAY_THRESH;//红方二值化阈值
	int BLUE_GRAY_THRESH;//蓝方二值化阈值
	int SPLIT_GRAY_THRESH;//通道分离二值化阈值

	long FAN_CONTOUR_AREA_MAX;//扇叶面积最大值
	long FAN_CONTOUR_AREA_MIN;//扇叶面积最小值
	long FAN_CONTOUR_LENGTH_MIN;//扇叶长边长度最小值
    long FAN_CONTOUR_LENGTH_MAX;//扇叶长边长度最大值
	long FAN_CONTOUR_WIDTH_MIN;//扇叶宽边长度最小值
    long FAN_CONTOUR_WIDTH_MAX;//扇叶宽边长度最大值
	float FAN_CONTOUR_HW_RATIO_MAX;//扇叶长宽比最大值
	float FAN_CONTOUR_HW_RATIO_MIN;//扇叶长宽比最小值
    float FAN_CONTOUR_AREA_RATIO_MIN;//装甲板轮廓占旋转矩形面积比最小值

	long ARMOR_CONTOUR_AREA_MAX;//装甲板面积最大值
	long ARMOR_CONTOUR_AREA_MIN;//装甲板面积最小值
	long ARMOR_CONTOUR_LENGTH_MIN;//装甲板长边长度最小值
	long ARMOR_CONTOUR_WIDTH_MIN;//装甲板长边长度最大值
	long ARMOR_CONTOUR_LENGTH_MAX;//装甲板宽边长度最小值
	long ARMOR_CONTOUR_WIDTH_MAX;//装甲板宽边长度最大值
	float ARMOR_CONTOUR_HW_RATIO_MAX;//装甲板长宽比最大值
	float ARMOR_CONTOUR_HW_RATIO_MIN;//装甲板长宽比最小值
	float ARMOR_CONTOUR_AREA_RATIO_MIN;//装甲板轮廓占旋转矩形面积比最小值

    long CENTER_R_CONTOUR_AREA_MAX;//风车中心R面积最大值
    long CENTER_R_CONTOUR_AREA_MIN;//风车中心R面积最小值
    long CENTER_R_CONTOUR_LENGTH_MIN;//风车中心R长边长度最小值
    long CENTER_R_CONTOUR_WIDTH_MIN;//风车中心R长边长度最大值
    long CENTER_R_CONTOUR_LENGTH_MAX;//风车中心R宽边长度最小值
    long CENTER_R_CONTOUR_WIDTH_MAX;//风车中心R宽边长度最大值
    float CENTER_R_CONTOUR_HW_RATIO_MAX;//风车中心R长宽比最大值
    float CENTER_R_CONTOUR_HW_RATIO_MIN;//风车中心R长宽比最小值
    float CENTER_R_CONTOUR_AREA_RATIO_MIN;//装甲板轮廓占旋转矩形面积比最小值
    float CENTER_R_CONTOUR_INTERSETION_AREA_MIN;//中心R占ROI区的面积最小值

    long FLOW_STRIP_FAN_CONTOUR_AREA_MAX;//流动条扇叶（待击打）面积最大值
    long FLOW_STRIP_FAN_CONTOUR_AREA_MIN;//流动条扇叶（待击打）面积最小值
    long FLOW_STRIP_FAN_CONTOUR_LENGTH_MIN;//流动条扇叶（待击打）长边长度最小值
    long FLOW_STRIP_FAN_CONTOUR_WIDTH_MIN;//流动条扇叶（待击打）宽边长度最小值
    long FLOW_STRIP_FAN_CONTOUR_LENGTH_MAX;//流动条扇叶（待击打）长边长度最大值
    long FLOW_STRIP_FAN_CONTOUR_WIDTH_MAX;//流动条扇叶（待击打）宽边长度最大值
    float FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MAX;//流动条扇叶（待击打）长宽比最大值
    float FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MIN;//流动条扇叶（待击打）长宽比最小值
    float FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MAX;//流动条扇叶轮廓占旋转矩形面积比最小值
    float FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MIN;//流动条扇叶占旋转矩形面积比最小值

    long FLOW_STRIP_CONTOUR_AREA_MAX;//流动条（待击打）面积最大值
    long FLOW_STRIP_CONTOUR_AREA_MIN;//流动条（待击打）面积最小值
    long FLOW_STRIP_CONTOUR_LENGTH_MIN;//流动条（待击打）长边长度最小值
    long FLOW_STRIP_CONTOUR_WIDTH_MIN;//流动条（待击打）宽边长度最小值
    long FLOW_STRIP_CONTOUR_LENGTH_MAX;//流动条（待击打）长边长度最大值
    long FLOW_STRIP_CONTOUR_WIDTH_MAX;//流动条（待击打）宽边长度最大值
    float FLOW_STRIP_CONTOUR_HW_RATIO_MAX;//流动条（待击打）长宽比最大值
    float FLOW_STRIP_CONTOUR_HW_RATIO_MIN;//流动条（待击打）长宽比最小值
    float FLOW_STRIP_CONTOUR_AREA_RATIO_MIN;//流动条占旋转矩形面积比最小值
    float FLOW_STRIP_CONTOUR_INTERSETION_AREA_MIN;//流动条占旋转矩形面积比最小值

	float TWIN_ANGEL_MAX;//两个理论上相等的角度在计算时具有的可能最大差值
	long TARGET_INTERSETION_CONTOUR_AREA_MIN;//扇叶与装甲板匹配时的最小重合面积

	long TWIN_POINT_MAX;//两个点相同时距离最大值

    long STRIP_ARMOR_DISTANCE_MIN;//流动条中心和目标装甲板中心距离最小值
    long STRIP_ARMOR_DISTANCE_MAX;//流动条中心和目标装甲板中心距离最大值
};

class Energy {
public:
    Energy(){}
    ~Energy(){}//默认析构函数
    EnergyPartParam energy_part_param_;//能量机关的参数设置

    cv::RotatedRect centerR;//风车中心字母R的可能候选区
    cv::RotatedRect flow_strip;//图像中所有流动条（理论上只有一个）
    cv::RotatedRect flow_strip_fan;//图像中所有流动条所在扇叶（理论上只有一个）
    cv::RotatedRect center_ROI;//风车中心候选区
    cv::RotatedRect target_armor;//目标装甲板（理论上仅一个）

    cv::Point circle_center_point;//风车圆心坐标
    cv::Point target_point;//目标装甲板中心坐标
    cv::Point guess_point;
    cv::Point predict_point;//预测的击打点坐标

    cv::Mat src_blue, src_red, src_green;//通道分离中的三个图像通道

    std::vector<cv::RotatedRect> fans;//图像中所有扇叶
    std::vector<cv::RotatedRect> armors;//图像中所有可能装甲板（可能存在误识别)
    std::vector<cv::RotatedRect> flow_strip_fans;//可能的流动扇叶
    std::vector<cv::RotatedRect> target_armors;//可能的目标装甲板
    std::vector<cv::RotatedRect> flow_strips;//可能的流动条
    std::vector<cv::Point> all_target_armor_centers;//记录全部的装甲板中心，用于风车圆心和半径的计算

    std::queue<float> recent_target_armor_centers;//记录最近一段时间的装甲板中心，用于判断大符还是小符


    void initEnergy();//能量机关初始化
    void initEnergyPartParam();
    void initImage(cv::Mat &src);//
    int findArmors(const cv::Mat &src);//
    void ArmorStruct(cv::Mat &src);
    bool isValidArmorContour(const vector<cv::Point> &armor_contour);
    void showArmors(std::string windows_name, const cv::Mat &src);
   
};

#endif //ENERGY_H

