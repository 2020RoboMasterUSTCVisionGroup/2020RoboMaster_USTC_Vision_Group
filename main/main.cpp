/*-----------------------------文件--------------------
*   文件名：main.cpp
*   作者：  孙霖
*   功能：  主函数
------------------------------------------------------*/
//------------------------包含文件和命名空间-------------------  
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MvCameraControl.h"

using namespace cv;
using namespace std;

#define MAX_IMAGE_DATA_SIZE   (1920*1200*3)
int cameraInit();
int cameraExit();
int lightBox(Mat image);
//------------------------全局变量定义---------------------
//变量定义阶段
extern int nRet;
extern void* handle;
//-----------------------主函数定义------------------------
int main(int argc, char *argv[], char **env)
{    	
	
    if(cameraInit()==0)
    {
        cout<<"cameraInit Failed."<<endl;
    }
	
    //获取一帧数据的大小
    MVCC_INTVALUE stIntvalue = {0};
    nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stIntvalue);

    if (nRet != MV_OK)
    {
        printf("Get PayloadSize failed! nRet [%x]\n", nRet);
    }	
	//一帧数据大小
	int nBuffSize = MAX_IMAGE_DATA_SIZE;
    unsigned int nTestFrameSize = 0;
    unsigned char* pFrameBuf = NULL;
    pFrameBuf = (unsigned char*)malloc(nBuffSize);

    //
    MV_FRAME_OUT_INFO_EX stInfo;
    memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    while(1)
    {
        nRet = MV_CC_GetImageForBGR(handle, pFrameBuf, nBuffSize, &stInfo, 1000);
        if( nRet != 0 )
        {
            cout << "error:GetImageForRGB:" << setbase(16) << nRet << endl;
			break;
        }
        else
        {
            cout << "imagetype:" << setbase(16) << stInfo.enPixelType << endl;
 
			int width = stInfo.nWidth;
			int height = stInfo.nHeight;
 
			if (stInfo.enPixelType == PixelType_Gvsp_BGR8_Packed)
			{
				Mat pImg(height, width, CV_8UC3, pFrameBuf);
				cvNamedWindow( "摄像头窗口", 1 ); 
                imshow("摄像头窗口", pImg);
                if( lightBox(pImg) == 1 )
                {
                    //printf("picture processed.\n");
                }
				waitKey(10);
			}
        }
        
    }
    cameraExit();

    return 0;
}
