//
// Created by xinyang on 19-3-27.
//
#ifndef _ARMOR_FINDER_H_
#define _ARMOR_FINDER_H_

#include<opencv2/opencv.hpp>
#include<string>
#include <Eigen/Core>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/tracking.hpp>
using namespace std;
using  namespace cv;
//----------------------------------------------------------------------------------------------------------------------
// 此结构体包括灯条参数
// ---------------------------------------------------------------------------------------------------------------------
struct BlobPartParam 
{
	int RED_GRAY_THRESH;
	int BLUE_GRAY_THRESH;
	int SPLIT_GRAY_THRESH;
    long BLOB_CONTOUR_AREA_MAX;//装甲板面积最大值
	long BLOB_CONTOUR_AREA_MIN;//装甲板面积最小值
	long BLOB_CONTOUR_LENGTH_MIN;//装甲板长边长度最小值
	long BLOB_CONTOUR_WIDTH_MIN;//装甲板长边长度最大值
	long BLOB_CONTOUR_LENGTH_MAX;//装甲板宽边长度最小值
	long BLOB_CONTOUR_WIDTH_MAX;//装甲板宽边长度最大值
	float BLOB_CONTOUR_HW_RATIO_MAX;//装甲板长宽比最大值
	float BLOB_CONTOUR_HW_RATIO_MIN;//装甲板长宽比最小值
	float BLOB_CONTOUR_AREA_RATIO_MIN;//装甲板轮廓占旋转矩形面积比最小值
};
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
    ArmorBox(const cv::Rect &pos=cv::Rect2d(), const LightBlobs &blobs=LightBlobs(), uint8_t color=1)
    :rect(pos),light_blobs(blobs),box_color(color){};

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
/******************
 * kalman
 * */
class Kalman{
	private:
		const int stateNum=4;                                      //状态值4×1向量(x,y,△x,△y)
	    const int measureNum=2;                                    //测量值2×1向量(x,y)	
		KalmanFilter KF ;	
		Mat prediction;
		Mat measurement ;  
		Point predict_pt ;
	public:
        Kalman(){
			this->KF=KalmanFilter(stateNum, measureNum, 0);
			this->KF.transitionMatrix = (Mat_<float>(4, 4) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);  //转移矩阵A
			

			setIdentity(KF.measurementMatrix);                                             //测量矩阵H
			setIdentity(KF.processNoiseCov, Scalar::all(1e-5));                            //系统噪声方差矩阵Q
			setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));                        //测量噪声方差矩阵R
			setIdentity(KF.errorCovPost, Scalar::all(1));                                  //后验错误估计协方差矩阵P
			//rng.fill(K input(200,300);ePost,RNG::UNIFORM,0,winHeight>winWidth?winWidth:winHeight);   //初始状态值x(0)
			randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
		}
		Point predict(Point input){
		    this->measurement=Mat::zeros(measureNum, 1, CV_32F); 
			this->prediction=this->KF.predict();
			this->predict_pt= Point(this->prediction.at<float>(0),this->prediction.at<float>(1) );   //预测值(x',y')
			//3.update measurement
			measurement.at<float>(0) = (float)input.x;
			measurement.at<float>(1) = (float)input.y;		
			//4.update
			KF.correct(measurement);

			// cout<<"point:"<<input.x<<","<<input.y<<endl;
			// cout<<"predit_point:"<<predict_pt.x<<","<<predict_pt.y<<endl;
            return Point(predict_pt.x,predict_pt.y);
		}

};


//
/********************* 自瞄类定义 **********************/
class AutoAiming{
private:       
    cv::Ptr<cv::Tracker> tracker;                       // tracker对象实例
    ArmorBox target_box, last_box;  //目标装甲板 上一个装甲板
    int tracking_cnt;
    int contour_area;                                   // 装甲区域亮点个数，用于数字识别未启用时判断是否跟丢（已弃用）

    //函数
    bool stateTrackingTarget(cv::Mat &src);
    bool findArmorBoxTop(cv::Mat &g_srcImage,cv::Mat &g_processImage,ArmorBox &target_box);
    bool stateSearchingTarget(cv::Mat &g_srcImage,cv::Mat &g_processImage);
    bool stateTrackingTarget(cv::Mat &g_srcImage,cv::Mat &g_processImage);
    bool findLightBolbsSJTU(cv::Mat &g_srcImage,cv::Mat &g_processImage,LightBlobs &light_blobs);
    bool matchArmorBoxes(const cv::Mat &src, const LightBlobs &light_blobs, ArmorBoxes &armor_boxes);
    void drawLightBlobs(cv::Mat &g_srcImage, const LightBlobs &blobs);
    void showLightBlobs(const cv::Mat &input_image,string windows_name,const LightBlobs &light_blobs);
    void showArmorBoxes(std::string windows_name, const cv::Mat &src, const ArmorBoxes &armor_boxes);
    void showArmorBox(std::string windows_name, const cv::Mat &g_srcImage, const cv::Rect2d &armor_box);
public:
    //战车状态定义
    typedef enum{
        SEARCHING_STATE, TRACKING_STATE, STANDBY_STATE
    } State;
    State state;
    Kalman *kf;

    void run(cv::Mat &g_srcImage,cv::Mat &g_processImage);
};
//此处为灯条颜色，对应通道分离可按照此处更改
#define BLOB_RED 1
#define BLOB_BLUE 0
#define ENEMY_RED 1
#define ENEMY_BLUE 0
/**
 * 此处函数可单独建立一个控制流程的类封装，
 * 将lightbolbs以及armorbox的对象也存放在该类中
 * */
bool findLightBolbsCSDN(Mat &input_img);
#endif _ARMOR_FINDER_H_ 
