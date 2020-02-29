/*-----------------------------文件--------------------
*   文件名：Preprocess.cpp
*   作者：  孙霖
*   功能：  载入图像预处理，目前实现对日光滤出
------------------------------------------------------*/

#include "Preprocess.h"

char EnemyColor='r';
int blockSize = 5;     //自适应阈值法的参数
int constValue =10;
void Preprocess::run(cv::Mat &src)
{
    clearWhiteLight(src);
    cvtColor(src, src, COLOR_GRAY2RGB);     //将黑白图像转换成三通道
    adaptiveThreshold(src,src, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);//自适应阈值法二值化
    uchar thresh=Otsu(src);
    threshold(src,src,thresh,255,CV_THRESH_OTSU);
}


void Preprocess::clearWhiteLight(Mat &src)
{
    Mat src_blue,src_red;       //红蓝通道图像
    if (src.type() == CV_8UC3){
        vector<Mat> channels;                 /*利用vector对象拆分*/
    split(src, channels);                     /*调用通道拆分函数*/
    { 
        src_red = channels[2];             /*将红色提出来，红色是第三个通道*/   
        src_blue = channels[0];            /*将蓝色提出来，红色是第一个通道*/  
    } 
        if(EnemyColor=='r')
        addWeighted(src_red, 1, src_blue,-1, 0.0, src);     //将两张图片按比例合成一张图片
        else 
        addWeighted(src_red, -1, src_blue,1, 0.0, src);     //蓝减红        
        
    }
}
//计算大津法的参数thresh
 uchar Otsu(Mat &img)
{
	int i,j;
	//行列
	int row = img.rows;
	int col = img.cols;
	double rc = row*col;
	//存储各级灰度的个数 初始为0
	int n[256] = {0};
	uchar *ptr;
	for(i = 0; i < row; ++i)  
    {  
        ptr = img.ptr<uchar>(i);  
        for (j = 0; j < col; ++j)  
        {  
            n[ptr[j]]++;
        }  
    }  
	
	//归一化 并且 计算累积和 、 累计均值 和 全局均值 
	double p[256];   
	double m[256];
	double mg;
 
	//计算概率的时候注意 如果都是整型 结果为0 
	p[0] = n[0]/rc;
	m[0] = 0;
	mg = 0;
 
    for(i = 1;i < 256; i++)
	{
		//计算概率 求全局均值
		p[i] = n[i]/rc;
		m[i] = i*p[i];
		mg += m[i];
        //进行累加
        p[i]+=p[i-1];
		m[i]+=m[i-1];
	}
 
	//阈值
	uchar k = 0;
	//类间方差 
	double a = 0,a2;
	for(i = 0;i < 256; i++)
	{       //这里少加个判断 if(p[i]==0||p[i]==1) continue;
		a2 = (mg*p[i]-m[i])*(mg*p[i]-m[i])/(p[i]*(1-p[i]));    
		if(a2 > a)
		{
			k = i;
			a = a2;
		}
	}
	return k;
}