
/**
*@author:王妍璐 江培玲
*@function:对图片进行通道拆分和腐蚀膨胀以提取出灯条
*/
#include <opencv2/highgui.hpp>     
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;                           /*之后程序中使用cv和std命名空间的资源时不用加前缀*/
using namespace std;
    
int main()
{
    Mat image1,red_channel,diffimg,afterprc;  /*创建图像容器*/
    Mat image = imread("before.png");         /*将项目中的before.png图像读入到image中*/
    imshow("原图",image);                   /*显示处理前的图像*/
    image1 = image;
    int val;
    for (int i = 0; i<image1.rows; i++)       /*每个像素每个通道的值减40*/
    {
        Vec3b* p1 = image.ptr<Vec3b>(i);
        Vec3b* p2 = image1.ptr<Vec3b>(i);
        for (int j = 0; j <image.cols; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                val = (int)(1 * p1[j][k] -40); 
                if (val < 0)
                val = 0; 
                if (val > 255) 
                val = 255;
                p2[j][k] = val;  
            }
        }
    }
    imshow("亮度调整后",image1);   
    vector<Mat> channels;                        /*利用vector对象拆分*/
    split(image1, channels);                     /*调用通道拆分函数*/
    { 
         
        red_channel = channels[2];               /*将红色提出来，红色是第三个通道*/   
    } 
    imshow("提取红色通道后",red_channel);             
    threshold(red_channel,diffimg,200, 255, CV_THRESH_BINARY);               /*调用二值化函数得到二值图*/
    imshow("二值化后",diffimg);  
    static Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(7, 9));
    static Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(5, 7));/*返回指定形状和尺寸的结构元素*/
    erode(diffimg, diffimg, kernel_erode);                                   
    dilate(diffimg,diffimg , kernel_dilate);                                 /*先腐蚀后膨胀,开运算*/
    dilate(diffimg, diffimg, kernel_dilate);
    erode(diffimg, afterprc, kernel_erode);                                  /*先膨胀后腐蚀,闭运算*/
    imshow("开闭运算后", afterprc);                                               /*输出图像*/
    while(1)
    {
        int key=waitKey(10); 
        if (key==27)                                                         /*按ESC键退出循环*/
        break;
    }
                                                                
    return 0;
}
