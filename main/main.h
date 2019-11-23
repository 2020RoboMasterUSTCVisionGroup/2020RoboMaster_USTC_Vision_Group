//------------------------包含文件和命名空间-------------------  
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MvCameraControl.h"

//包含自制头文件
#include "cameraInit.h"

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
int g_source_image;
extern int nRet;    //相机状态定义
extern void* handle;    //
Mat g_srcImage;     //原图像每一帧
VideoCapture g_capture;//视频文件
//一帧数据大小
int nBuffSize = MAX_IMAGE_DATA_SIZE;
unsigned int nTestFrameSize = 0;
unsigned char* pFrameBuf = NULL;
MV_FRAME_OUT_INFO_EX stInfo;