#include "../inc/energy.h"
#define show_process 1

//----------------------------------------------------------------------------------------------------------------------
// 此函数对能量机关参数进行初始化
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initEnergyPartParam() {
    energy_part_param_.RED_GRAY_THRESH = 180;//game
    energy_part_param_.BLUE_GRAY_THRESH = 100;//game
    energy_part_param_.SPLIT_GRAY_THRESH = 180;

    energy_part_param_.FAN_CONTOUR_AREA_MAX = 5000;
    energy_part_param_.FAN_CONTOUR_AREA_MIN = 1500;
    energy_part_param_.FAN_CONTOUR_LENGTH_MIN = 45;
    energy_part_param_.FAN_CONTOUR_LENGTH_MAX = 100;
    energy_part_param_.FAN_CONTOUR_WIDTH_MIN = 10;
    energy_part_param_.FAN_CONTOUR_WIDTH_MAX = 52;
    energy_part_param_.FAN_CONTOUR_HW_RATIO_MAX = 3.5;
    energy_part_param_.FAN_CONTOUR_HW_RATIO_MIN = 1.2;
    energy_part_param_.FAN_CONTOUR_AREA_RATIO_MIN = 0.6;

    energy_part_param_.ARMOR_CONTOUR_AREA_MAX = 500;
    energy_part_param_.ARMOR_CONTOUR_AREA_MIN = 180;
    energy_part_param_.ARMOR_CONTOUR_LENGTH_MIN = 10;
    energy_part_param_.ARMOR_CONTOUR_LENGTH_MAX = 50;
    energy_part_param_.ARMOR_CONTOUR_WIDTH_MIN = 0;
    energy_part_param_.ARMOR_CONTOUR_WIDTH_MAX = 30;
    energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MAX = 3;
    energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MIN = 1;

    energy_part_param_.CENTER_R_CONTOUR_AREA_MAX = 200;
    energy_part_param_.CENTER_R_CONTOUR_AREA_MIN = 40;
    energy_part_param_.CENTER_R_CONTOUR_LENGTH_MIN = 6;
    energy_part_param_.CENTER_R_CONTOUR_LENGTH_MAX = 20;
    energy_part_param_.CENTER_R_CONTOUR_WIDTH_MIN = 6;
    energy_part_param_.CENTER_R_CONTOUR_WIDTH_MAX = 20;
    energy_part_param_.CENTER_R_CONTOUR_HW_RATIO_MAX = 2;
    energy_part_param_.CENTER_R_CONTOUR_HW_RATIO_MIN = 1;
    energy_part_param_.CENTER_R_CONTOUR_AREA_RATIO_MIN = 0.6;
    energy_part_param_.CENTER_R_CONTOUR_INTERSETION_AREA_MIN = 10;

    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_AREA_MAX = 2000;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_AREA_MIN = 500;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_LENGTH_MIN = 60;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_LENGTH_MAX = 100;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_WIDTH_MIN = 20;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_WIDTH_MAX = 52;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MAX = 2.8;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MIN = 1.2;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MAX = 0.58;
    energy_part_param_.FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MIN = 0.34;

    energy_part_param_.FLOW_STRIP_CONTOUR_AREA_MAX = 700;
    energy_part_param_.FLOW_STRIP_CONTOUR_AREA_MIN = 100;
    energy_part_param_.FLOW_STRIP_CONTOUR_LENGTH_MIN = 32;
    energy_part_param_.FLOW_STRIP_CONTOUR_LENGTH_MAX = 55;
    energy_part_param_.FLOW_STRIP_CONTOUR_WIDTH_MIN = 4;
    energy_part_param_.FLOW_STRIP_CONTOUR_WIDTH_MAX = 20;
    energy_part_param_.FLOW_STRIP_CONTOUR_HW_RATIO_MAX = 7;
    energy_part_param_.FLOW_STRIP_CONTOUR_HW_RATIO_MIN = 3;
    energy_part_param_.FLOW_STRIP_CONTOUR_AREA_RATIO_MIN = 0.6;
    energy_part_param_.FLOW_STRIP_CONTOUR_INTERSETION_AREA_MIN = 100;

    energy_part_param_.TWIN_ANGEL_MAX = 10;
    energy_part_param_.TARGET_INTERSETION_CONTOUR_AREA_MIN = 40;

    energy_part_param_.TWIN_POINT_MAX = 20;

    energy_part_param_.STRIP_ARMOR_DISTANCE_MIN = 28;
    energy_part_param_.STRIP_ARMOR_DISTANCE_MAX = 52;
}
//----------------------------------------------------------------------------------------------------------------------
// 此函数用于图像预处理
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initImage(cv::Mat &src) {
    if (src.type() == CV_8UC3){
        cvtColor(src, src, COLOR_BGR2GRAY);
    }
    if (ENEMY_BLUE){
        threshold(src, src, 200, 255, THRESH_BINARY);
    } else if(ENEMY_RED){
        threshold(src, src, 200, 255, THRESH_BINARY);
    }
    if (show_process){
        imshow("bin", src);
        waitKey(0);
    } 
}
//----------------------------------------------------------------------------------------------------------------------
// 此函数对图像进行腐蚀与膨胀操作
// ---------------------------------------------------------------------------------------------------------------------
void Energy::ArmorStruct(cv::Mat &src) {
    Mat element_dilate_1 = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element_erode_1 = getStructuringElement(MORPH_RECT, Size(2, 2));
    Mat element_dilate_2 = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat element_erode_2 = getStructuringElement(MORPH_RECT, Size(2 , 2));
    Mat element_dilate_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat element_erode_3 = getStructuringElement(MORPH_RECT, Size(1 , 1));

    dilate(src, src, element_dilate_1);
    erode(src,src, element_erode_1);
}
//----------------------------------------------------------------------------------------------------------------------
// 此函数用于判断找到的矩形候选区是否为装甲板
// ---------------------------------------------------------------------------------------------------------------------
bool Energy::isValidArmorContour(const vector<cv::Point> &armor_contour) {
    double cur_contour_area = contourArea(armor_contour);
    if (cur_contour_area > energy_part_param_.ARMOR_CONTOUR_AREA_MAX ||
        cur_contour_area < energy_part_param_.ARMOR_CONTOUR_AREA_MIN) {
        //cout<<cur_contour_area<<" "<<energy_fan_param_.CONTOUR_AREA_MIN<<" "<<energy_fan_param_.CONTOUR_AREA_MAX<<endl;
        //cout<<"area fail."<<endl;
        return false;
        //选区面积大小不合适
    }
    RotatedRect cur_rect = minAreaRect(armor_contour);
    Size2f cur_size = cur_rect.size;
    float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;//将矩形的长边设置为长
    float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;//将矩形的短边设置为宽
    if (length < energy_part_param_.ARMOR_CONTOUR_LENGTH_MIN || width < energy_part_param_.ARMOR_CONTOUR_WIDTH_MIN ||
        length > energy_part_param_.ARMOR_CONTOUR_LENGTH_MAX || width > energy_part_param_.ARMOR_CONTOUR_WIDTH_MAX) {
        //cout<<"length width fail."<<endl;
//        cout << "length: " << length << '\t' << "width: " << width << '\t' << cur_rect.center << endl;
        return false;
        //矩形边长不合适
    }

    float length_width_ratio = length / width;//计算矩形长宽比
    if (length_width_ratio > energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MAX ||
        length_width_ratio < energy_part_param_.ARMOR_CONTOUR_HW_RATIO_MIN) {
        //cout<<"length width ratio fail."<<endl;
//        cout << "HW: " << length_width_ratio << '\t' << cur_rect.center << endl;
        return false;
        //长宽比不合适
    }
    if (cur_contour_area / cur_size.area() < energy_part_param_.ARMOR_CONTOUR_AREA_RATIO_MIN) {
//        cout << "area ratio: " << cur_contour_area / cur_size.area() << '\t' << cur_rect.center << endl;
        return false;//轮廓对矩形的面积占有率不合适
    }
    return true;
}
//----------------------------------------------------------------------------------------------------------------------
// 此函数用于寻找图像内所有的大风车装甲板模块
// ---------------------------------------------------------------------------------------------------------------------
int Energy::findArmors(const cv::Mat &src) {
    if (src.empty()) {
        cout << "empty!" << endl;
        return 0;
    }
    static Mat src_bin;
    src_bin = src.clone();
    if (src.type() == CV_8UC3) {
        cvtColor(src_bin, src_bin, CV_BGR2GRAY);//若读取三通道视频文件，需转换为单通道
    }
    std::vector<vector<Point> > armor_contours;
    std::vector<vector<Point> > armor_contours_external;//用总轮廓减去外轮廓，只保留内轮廓，除去流动条的影响。

    ArmorStruct(src_bin);//图像膨胀，防止图像断开并更方便寻找
    findContours(src_bin, armor_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    if (show_process)imshow("armor struct", src_bin);

    findContours(src_bin, armor_contours_external, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (int i = 0; i < armor_contours_external.size(); i++)//去除外轮廓
    {
        unsigned long external_contour_size = armor_contours_external[i].size();
        for (int j = 0; j < armor_contours.size(); j++) {
            unsigned long all_size = armor_contours[j].size();
            if (external_contour_size == all_size) {
                swap(armor_contours[j], armor_contours[armor_contours.size() - 1]);
                armor_contours.pop_back();
                break;
            }
        }
    }

    for (auto &armor_contour : armor_contours) {
        if (!isValidArmorContour(armor_contour)) {
            continue;
        }
        armors.emplace_back(cv::minAreaRect(armor_contour));
    }


    if (armors.size() < 1)cout << "no armors!" << endl;
    

    return static_cast<int>(armors.size());
}
//----------------------------------------------------------------------------------------------------------------------
// 此函数用于显示图像中所有装甲板
// ---------------------------------------------------------------------------------------------------------------------
void Energy::showArmors(std::string windows_name, const cv::Mat &src) {
    if (src.empty())return;
    static Mat image2show;

    if (src.type() == CV_8UC1) // 黑白图像
    {
        cvtColor(src, image2show, COLOR_GRAY2RGB);

    } else if (src.type() == CV_8UC3) //RGB 彩色
    {
        image2show = src.clone();
    }
    for (const auto &armor : armors) {
        Point2f vertices[4];      //定义矩形的4个顶点
        armor.points(vertices);   //计算矩形的4个顶点
        for (int i = 0; i < 4; i++)
            line(image2show, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
    }
     imshow(windows_name, image2show);
}
int main(){
    Mat test;
    Energy energy;
    test =imread("./test.png");
    energy.initEnergyPartParam();
    energy.initImage(test);
    energy.findArmors(test);
    energy.showArmors("res",test);
    return 0;

}
