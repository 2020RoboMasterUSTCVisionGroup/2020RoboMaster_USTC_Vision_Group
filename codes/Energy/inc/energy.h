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
    //Energy(Serial &u, uint8_t &color);//构造函数，参数为串口和敌方颜色
    ~Energy();//默认析构函数

    bool is_big;//大符模式为true
    bool is_small;//小符模式为true

private:
    EnergyPartParam energy_part_param_;//能量机关的参数设置

    bool is_guessing;//当前处于发弹到新目标出现的过程，则为true，此时猜测下一个目标
    bool is_predicting;//当前处于新目标出现到发弹的过程，则为true，此时正常击打
    bool energy_mode_init;//大小符状态判断
    bool energy_rotation_init;//若仍在判断风车旋转方向，则为true
    bool start_guess;//进入猜测状态的标志
    bool change_target;//目标切换的标志

    uint8_t &ally_color;//我方颜色

    int curr_fps;//帧率
    int send_cnt;//向主控板发送的数据总次数
    int fans_cnt;//扇叶个数
    int last_fans_cnt;//上一帧的扇叶个数
    int guess_devide;//刚进入猜测状态时，猜测目标点在极坐标中的分区
    int energy_rotation_direction;//风车旋转方向
    int clockwise_rotation_init_cnt;//装甲板顺时针旋转次数
    int anticlockwise_rotation_init_cnt;//装甲板逆时针旋转次数
    int last_mode;//上一帧的能量机关状态
    int manual_delta_x, manual_delta_y;//手动微调量
    int extra_delta_x, extra_delta_y;//在风车运动到最高点附近的额外补偿量

    float target_polar_angle;//待击打装甲板的极坐标角度
    float last_target_polar_angle_judge_change;//上一帧待击打装甲板的极坐标角度（用于判断目标切换）
    float last_target_polar_angle_judge_rotation;//上一帧待击打装甲板的极坐标角度（用于判断旋向）
    float guess_polar_angle;//猜测的下一个目标装甲板极坐标角度
    float last_base_angle;//上一帧的各扇叶在0区（0°~72°）的基础角度
    float predict_rad;//预测提前角
    float predict_rad_norm;//预测提前角的绝对值
    float attack_distance;//步兵与风车平面距离
    float center_delta_yaw, center_delta_pitch;//对心时相差的角度
    float yaw_rotation, pitch_rotation;//云台yaw轴和pitch轴应该转到的角度
    float shoot;//给主控板的指令，1表示跟随，2表示发射，3表示目标切换,4表示猜测模式
    float last_yaw, last_pitch;//PID中微分项
    float sum_yaw, sum_pitch;//yaw和pitch的累计误差，即PID中积分项

    //systime time_start_guess;//进入猜测模式的时间

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
   
};

#endif //ENERGY_H

