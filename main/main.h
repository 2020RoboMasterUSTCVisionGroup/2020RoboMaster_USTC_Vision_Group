#ifndef MAIN_H_
#define MAIN_H_
//------------------------包含文件和命名空间-------------------  
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include "MvCameraControl.h"

//包含自制头文件
#include "CameraInit.h"
#include "Preprocess.h"
#include "energy.h"
#include "ArmorFinder.h"
#include "options.h"  
#include "cans.h"
#include "addtions.h" 


using namespace cv;
using namespace std;


#define MAX_IMAGE_DATA_SIZE   (1920*1200*3)
#define SOURCE_CAMERA    1
#define SOURCE_VIDEO    0
#define CAMERA_INIT_SUCCESS 1
#define CAMERA_INIT_FAIL    0
int lightBox(Mat image);
void systemInit();
//------------------------全局变量定义---------------------
//变量定义阶段
int g_source_type;
VideoCapture g_capture;
Mat g_srcImage;     //原图像每一帧
Preprocess g_preprocess;    //初始化对象
//CAN
Can can;
extern void canReceive(Can *);
McuData mcu_data={    // 单片机端回传结构体初始化
            0,             // 当前大云台yaw角度
            0,              // 当前大云台pitch角度
            0,              // 当前小云台yaw角度
            0,              // 当前小云台pitch角度
            0,             // 子弹速度
            0,             // 光照强度
            0,              // 大/小云台工作模式 0x00：大云台直射，小云台全自动  0x01:大云台抛射，小云台全自动
            0,              // 前4位表示工作状态，后4位表示敌方战车颜色
            };
//default red
volatile int enemy_color=BLOB_RED;

//extern int nRet;    //相机状态定义
//extern void* handle;    //
//一帧数据大小
int nBuffSize = MAX_IMAGE_DATA_SIZE;
unsigned int nTestFrameSize = 0;
//相机位置
unsigned char* pFrameBuf = NULL;

//相机参数
MV_FRAME_OUT_INFO_EX stInfo;

unsigned char handshake[]= "2020";

#endif