#include "MvCameraControl.h"
#include <iostream>
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;
#define MAX_BUF_SIZE    (1920*1200*3)
 
 
int main()
{
	int nRet = -1;
	void* m_handle = NULL;
 
	//枚举子网内指定的传输协议对应的所有设备
	unsigned int nTLayerType = MV_USB_DEVICE;
	MV_CC_DEVICE_INFO_LIST m_stDevList = { 0 };
	nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);
	if (nRet != 0)
	{
		printf("error: EnumDevices fail [%x]\n", nRet);
	}
 
	if (m_stDevList.nDeviceNum == 0)
	{
		printf("no camera found!\n");
	}
 
	//选择查找到的第一台在线设备，创建设备句柄
	int nDeviceIndex = 0;
 
	MV_CC_DEVICE_INFO m_stDevInfo = { 0 };
	memcpy(&m_stDevInfo, m_stDevList.pDeviceInfo[nDeviceIndex], sizeof(MV_CC_DEVICE_INFO));
 
	nRet = MV_CC_CreateHandle(&m_handle, &m_stDevInfo);
 
	if (nRet != 0)
	{
		printf("error: CreateHandle fail [%x]\n", nRet);
	}
 
 
 
	//连接设备
	unsigned int nAccessMode = MV_ACCESS_Exclusive;
	unsigned short nSwitchoverKey = 0;
	nRet = MV_CC_OpenDevice(m_handle, nAccessMode, nSwitchoverKey);
	if (nRet != 0)
	{
		printf("error: OpenDevice fail [%x]\n", nRet);
	}
	//...其他处理 
 
	//开始采集图像
	nRet = MV_CC_StartGrabbing(m_handle);
	if (nRet != 0)
	{
		printf("error: StartGrabbing fail [%x]\n", nRet);
	}
 
	//获取一帧数据的大小
	MVCC_INTVALUE stIntvalue = { 0 };
	nRet = MV_CC_GetIntValue(m_handle, "PayloadSize", &stIntvalue);
	if (nRet != 0)
	{
		printf("Get PayloadSize failed! nRet [%x]\n", nRet);
	}
	//int nBufSize = stIntvalue.nCurValue; //一帧数据大小
	int nBufSize = MAX_BUF_SIZE;
	unsigned int    nTestFrameSize = 0;
	unsigned char*  pFrameBuf = NULL;
	pFrameBuf = (unsigned char*)malloc(nBufSize);
 
	MV_FRAME_OUT_INFO_EX stInfo;
	memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
 
 
	//上层应用程序需要根据帧率，控制好调用该接口的频率
	//此次代码仅供参考，实际应用建议另建线程进行图像帧采集和处理
	while (1)
	{
		if (nTestFrameSize > 50)
		{
			break;
		}
		nRet = MV_CC_GetImageForBGR(m_handle, pFrameBuf, nBufSize, &stInfo, 1000);
		if (nRet != 0)
		{
			cout << "error:GetImageForRGB:" << setbase(16) << nRet << endl;
			break;
		}
		else
		{
			//...图像数据处理
			cout << "imagetype:" << setbase(16) << stInfo.enPixelType << endl;
 
			int width = stInfo.nWidth;
			int height = stInfo.nHeight;
 
			if (stInfo.enPixelType == PixelType_Gvsp_BGR8_Packed)
			{
				cv::Mat pImg(height, width, CV_8UC3, pFrameBuf);
				cv::imshow("Carema", pImg);
				cv::waitKey(10);
			}
 
			
 
 
 
			nTestFrameSize++;
		}
	}
 
	//...其他处理
 
	//停止采集图像 
	nRet = MV_CC_StopGrabbing(m_handle);
	if (nRet != 0)
	{
		printf("error: StopGrabbing fail [%x]\n", nRet);
	}
 
	//关闭设备，释放资源
	nRet = MV_CC_CloseDevice(m_handle);
	if (nRet != 0)
	{
		printf("error: CloseDevice fail [%x]\n", nRet);
	}
 
	//销毁句柄，释放资源
	nRet = MV_CC_DestroyHandle(m_handle);
	if (nRet != 0)
	{
		printf("error: DestroyHandle fail [%x]\n", nRet);
	}
	system("pause");
}