
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "iostream"

using namespace cv;
using namespace std;

#define T_ANGLE_THRE 10
#define T_SIZE_THRE 5

void brightAdjust(Mat src, Mat dst, double dContrast, double dBright); //亮度调节函数
void getDiffImage(Mat src1, Mat src2, Mat dst, int nThre); //二值化
void drawBox(RotatedRect box, Mat img);
Mat channelSwitching(const Mat& src);

int main()
{
	Mat frame0 = imread("aa.png");
	Size imgSize;
    RotatedRect s;   //定义旋转矩形
    vector<RotatedRect> vEllipse; //定以旋转矩形的向量，用于存储发现的目标区域
   	bool bFlag = false;
	vector<vector<Point> > contour;
	imgSize =frame0.size();

	Mat rawImg = Mat(imgSize, CV_8UC3);
    Mat blt = Mat(imgSize, CV_8UC3);
    Mat grayImage = Mat(imgSize, CV_8UC1);
   	Mat rImage = Mat(imgSize, CV_8UC1);
	Mat gImage = Mat(imgSize, CV_8UC1);
    Mat bImage = Mat(imgSize, CV_8UC1);
    Mat binary = Mat(imgSize, CV_8UC1);
    Mat rlt = Mat(imgSize, CV_8UC1);
    namedWindow("Raw");
	
	brightAdjust(frame0, rawImg, 1, -120);  //每个像素每个通道的值都减去120
	Mat bgr[3];
 	split(rawImg, bgr); //将三个通道的像素值分离
    bImage = bgr[0];
    gImage = bgr[1];
    rImage = bgr[2];
     //如果像素R值-G值大于25，则返回的二值图像的值为255，否则为0

	getDiffImage(rImage, gImage, binary, 25); 
	dilate(binary, grayImage, Mat(), Point(-1,-1), 3);   //图像膨胀
	erode(grayImage, rlt, Mat(), Point(-1,-1), 1);  //图像腐蚀，先膨胀在腐蚀属于闭运算
	findContours(rlt, contour, RETR_CCOMP , CHAIN_APPROX_SIMPLE); //在二值图像中寻找轮廓

	for (int i=0; i<contour.size(); i++)
	{
                if (contour[i].size()> 10)  //判断当前轮廓是否大于10个像素点
                {
                    bFlag = true;   //如果大于10个，则检测到目标区域
                  //拟合目标区域成为椭圆，返回一个旋转矩形（中心、角度、尺寸）
                    s = fitEllipse(contour[i]);  
                    for (int nI = 0; nI < 5; nI++)
                    {
                        for (int nJ = 0; nJ < 5; nJ++)  //遍历以旋转矩形中心点为中心的5*5的像素块
                        {
                            if (s.center.y - 2 + nJ > 0 && s.center.y - 2 + nJ < 480 && s.center.x - 2 + nI > 0 && s.center.x - 2 + nI <  640)  //判断该像素是否在有效的位置
                            {   
                                Vec3b v3b = frame0.at<Vec3b>((int)(s.center.y - 2 + nJ), (int)(s.center.x - 2 + nI)); //获取遍历点点像素值
                               //判断中心点是否接近白色
                                if (v3b[0] < 200 || v3b[1] < 200 || v3b[2] < 200)
                                    bFlag = false;        //如果中心不是白色，则不是目标区域
                            }
                        }
                    }
	 if (bFlag)
                    {
                        vEllipse.push_back(s); //将发现的目标保存
                    }
                }

            }
    cvtColor(rlt,blt,CV_GRAY2BGR);
    for (unsigned int nI = 0; nI < vEllipse.size(); nI++) //在当前图像中标出灯条的位置
            drawBox(vEllipse[nI], blt);
    imshow("Raw", blt);
    imwrite("raw.png",blt);
    waitKey();
    return 0;
}
void brightAdjust(Mat src, Mat dst, double dContrast, double dBright)
{
    int nVal;
    

    for (int nI = 0; nI<src.rows; nI++)
    {
        Vec3b* p1 = src.ptr<Vec3b>(nI);
        Vec3b* p2 = dst.ptr<Vec3b>(nI);
        for (int nJ = 0; nJ <src.cols; nJ++)
        {
            for (int nK = 0; nK < 3; nK++)
            {
               //每个像素的每个通道的值都进行线性变换
                nVal = (int)(dContrast * p1[nJ][nK] + dBright);
                if (nVal < 0)
                    nVal = 0;
                if (nVal > 255)
                    nVal = 255;
                p2[nJ][nK] = nVal;
            }
        }
    }
}

void getDiffImage(Mat src1, Mat src2, Mat dst, int nThre)
{
    

    for (int nI = 0; nI<src1.rows; nI++)
    {
        uchar* pchar1 = src1.ptr<uchar>(nI);
        uchar* pchar2 = src2.ptr<uchar>(nI);
        uchar* pchar3 = dst.ptr<uchar>(nI);
        for (int nJ = 0; nJ <src1.cols; nJ++)
        {
            if (pchar1[nJ] - pchar2[nJ]> nThre) //
            {
                pchar3[nJ] = 255;
            }
            else
            {
                pchar3[nJ] = 0;
            }
        }
    }
}
void drawBox(RotatedRect box,Mat img)
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
