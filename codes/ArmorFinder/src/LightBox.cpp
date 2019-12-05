
/*-----------------------------文件--------------------
*   文件名：LightBox.cpp
*   作者：  王妍璐 江培玲
*   功能：  对图片进行通道拆分和腐蚀膨胀以提取出灯条
------------------------------------------------------*/
//-----------------------------头文件引用和命名空间-------------------
#include <opencv2/highgui.hpp>     
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;                           /*之后程序中使用cv和std命名空间的资源时不用加前缀*/
using namespace std;

#define T_ANGLE_THRE 10
#define T_SIZE_THRE 5
/**
*@author：王妍璐 江培玲
*@name：drawBox()
*@return:void
*@function：
*@para：box: img:
*其他要注意的地方
**/
void drawBox(RotatedRect &box,Mat &img)
{
    Point2f pt[4];
    int i;
    for (i = 0; i<4; i++)
    {
        pt[i].x = 0;
        pt[i].y = 0;
    }
    box.points(pt); //计算二维盒子顶点 
    line(img, pt[0], pt[1], CV_RGB(255, 0, 0), 2, 8, 0);
    line(img, pt[1], pt[2], CV_RGB(255, 0, 0), 2, 8, 0);
    line(img, pt[2], pt[3], CV_RGB(255, 0, 0), 2, 8, 0);
    line(img, pt[3], pt[0], CV_RGB(255, 0, 0), 2, 8, 0);
}
vector<RotatedRect> armorDetect(vector<RotatedRect> vEllipse)
{
    vector<RotatedRect> vRlt;
    RotatedRect armor; //定义装甲区域的旋转矩形
    int nL, nW;
    double dAngle;
    vRlt.clear();
    if (vEllipse.size() < 2) //如果检测到的旋转矩形个数小于2，则直接返回
        return vRlt;
    for (unsigned int nI = 0; nI < vEllipse.size() - 1; nI++) //求任意两个旋转矩形的夹角
    {
        for (unsigned int nJ = nI + 1; nJ < vEllipse.size(); nJ++)
        {
            dAngle = abs(vEllipse[nI].angle - vEllipse[nJ].angle);
            while (dAngle > 180)
                dAngle -= 180;
          //判断这两个旋转矩形是否是一个装甲的两个LED等条
            if ((dAngle < T_ANGLE_THRE || 180 - dAngle < T_ANGLE_THRE) && abs(vEllipse[nI].size.height - vEllipse[nJ].size.height) < (vEllipse[nI].size.height + vEllipse[nJ].size.height) / T_SIZE_THRE && abs(vEllipse[nI].size.width - vEllipse[nJ].size.width) < (vEllipse[nI].size.width + vEllipse[nJ].size.width) / T_SIZE_THRE) 
            {
                armor.center.x = (vEllipse[nI].center.x + vEllipse[nJ].center.x) / 2; //装甲中心的x坐标 
                armor.center.y = (vEllipse[nI].center.y + vEllipse[nJ].center.y) / 2; //装甲中心的y坐标
                armor.angle = (vEllipse[nI].angle + vEllipse[nJ].angle) / 2;   //装甲所在旋转矩形的旋转角度
                if (180 - dAngle < T_ANGLE_THRE)
                    armor.angle += 90;
                nL = (vEllipse[nI].size.height + vEllipse[nJ].size.height) / 2; //装甲的高度
                nW = sqrt((vEllipse[nI].center.x - vEllipse[nJ].center.x) * (vEllipse[nI].center.x - vEllipse[nJ].center.x) + (vEllipse[nI].center.y - vEllipse[nJ].center.y) * (vEllipse[nI].center.y - vEllipse[nJ].center.y)); //装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
                if (nL < nW)
                {
                    armor.size.height = nL;
                    armor.size.width = nW;
                }
                else
                {
                    armor.size.height = nW;
                    armor.size.width = nL;
                }
                vRlt.push_back(armor); //将找出的装甲的旋转矩形保存到vector
            }
        }
    }
    return vRlt;
}
/**
*@author：王妍璐 江培玲
*@name：main()
*@return:void
*@function：
*@para：box: img:
*其他要注意的地方
**/  
int lightBox(Mat image)
{  
    std::cout<<"lightbox start"<<std::endl;
    Mat image1,red_channel,diffimg,afterprc,light_loc;  /*创建图像容器*/         /*将项目中的before.png图像读入到image中*/
    vector<vector<Point> > contour;           /*定义二维浮点型变量存放找到的边界坐标*/
    bool bFlag = false;
    RotatedRect s;                            /*定义旋转矩形*/
    vector<RotatedRect> vEllipse;             /*定以旋转矩形的向量，用于存储发现的目标区域*/
    vector<RotatedRect> vRlt;
    //imshow("原图",image);                   /*显示处理前的图像*/
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
    //imshow("亮度调整后",image1);   
    vector<Mat> channels;                        /*利用vector对象拆分*/
    split(image1, channels);                     /*调用通道拆分函数*/
    { 
         
        red_channel = channels[2];               /*将红色提出来，红色是第三个通道*/   
    } 
    //imshow("提取红色通道后",red_channel);             
    threshold(red_channel,diffimg,200, 255, CV_THRESH_BINARY);               /*调用二值化函数得到二值图*/
    //imshow("二值化后",diffimg);  
    static Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(7, 9));
    static Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(5, 7));/*返回指定形状和尺寸的结构元素*/
    erode(diffimg, diffimg, kernel_erode);                                   
    dilate(diffimg,diffimg , kernel_dilate);                                 /*先腐蚀后膨胀,开运算*/
    dilate(diffimg, diffimg, kernel_dilate);
    erode(diffimg, afterprc, kernel_erode);                                  /*先膨胀后腐蚀,闭运算*/
    //imshow("开闭运算后", afterprc);                              /*输出图像*/
    findContours(afterprc, contour, RETR_CCOMP , CHAIN_APPROX_SIMPLE);       //在二值图像中寻找轮廓
	for (int i=0; i<contour.size(); i++)
	{
                if (contour[i].size()> 10)                 //判断当前轮廓是否大于10个像素点
                {
                    bFlag = true;                          //如果大于10个，则检测到目标区域
                                                           //拟合目标区域成为椭圆，返回一个旋转矩形（中心、角度、尺寸）
                    s = fitEllipse(contour[i]);  
                    for (int nI = 0; nI < 5; nI++)
                    {
                        for (int nJ = 0; nJ < 5; nJ++)     //遍历以旋转矩形中心点为中心的5*5的像素块
                        {
                            if (s.center.y - 2 + nJ > 0 && s.center.y - 2 + nJ < 480 && s.center.x - 2 + nI > 0 && s.center.x - 2 + nI <  640)  //判断该像素是否在有效的位置
                            {   
                                Vec3b v3b = image.at<Vec3b>((int)(s.center.y - 2 + nJ), (int)(s.center.x - 2 + nI)); //获取遍历点点像素值
                                                          //判断中心点是否接近白色
                                if (v3b[0] < 200 || v3b[1] < 200 || v3b[2] < 200)
                                    bFlag = false;        //如果中心不是白色，则不是目标区域
                            }
                        }
                    }
	 if (bFlag)
                    {
                        vEllipse.push_back(s);            //将发现的目标保存
                    }
                }

            }
    vRlt = armorDetect(vEllipse); 
   // cvtColor(afterprc,light_loc,CV_GRAY2BGR);
    for (unsigned int nI = 0; nI < vRlt.size(); nI++) //在当前图像中标出灯条的位置
    {
       // drawBox(vEllipse[nI], light_loc);
       drawBox(vRlt[nI], image);
    } 
    imshow("圈出位置", image);
    waitKey(10);                                                        
    return 1;
}