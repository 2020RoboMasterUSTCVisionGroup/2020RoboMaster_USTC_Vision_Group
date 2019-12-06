#include "ArmorFinder.h"
// 旋转矩形的长宽比
static double lw_rate(const cv::RotatedRect &rect) {
    return rect.size.height > rect.size.width ?
           rect.size.height / rect.size.width :
           rect.size.width / rect.size.height;
}
// 轮廓面积和其最小外接矩形面积之比
static double areaRatio(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect) {
    return cv::contourArea(contour) / rect.size.area();
}
// 判断轮廓是否为一个灯条
static bool isValidLightBlob(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect) {
    return (1.2 < lw_rate(rect) && lw_rate(rect) < 10) &&
           //           (rect.size.area() < 3000) &&
           ((rect.size.area() < 50 && areaRatio(contour, rect) > 0.4) ||
            (rect.size.area() >= 50 && areaRatio(contour, rect) > 0.6));
}
// 判断灯条颜色(此函数可以有性能优化).
static uint8_t get_blob_color(const cv::Mat &src, const cv::RotatedRect &blobPos) {
    auto region = blobPos.boundingRect();
    region.x -= fmax(3, region.width * 0.1);
    region.y -= fmax(3, region.height * 0.05);
    region.width += 2 * fmax(3, region.width * 0.1);
    region.height += 2 * fmax(3, region.height * 0.05);
    region &= cv::Rect(0, 0, src.cols, src.rows);
    cv::Mat roi = src(region);
    int red_cnt = 0, blue_cnt = 0;
    for (int row = 0; row < roi.rows; row++) {
        for (int col = 0; col < roi.cols; col++) {
            red_cnt += roi.at<cv::Vec3b>(row, col)[2];
            blue_cnt += roi.at<cv::Vec3b>(row, col)[0];
        }
    }
    if (red_cnt > blue_cnt) {
        return BLOB_RED;
    } else {
        return BLOB_BLUE;
    }
}
// 判断两个灯条区域是同一个灯条
static bool isSameBlob(LightBlob blob1, LightBlob blob2) {
    auto dist = blob1.rect.center - blob2.rect.center;
    return (dist.x * dist.x + dist.y * dist.y) < 9;
}
// 开闭运算
static void imagePreProcess(cv::Mat &src) {
    static cv::Mat kernel_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode);

    static cv::Mat kernel_dilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate);

    static cv::Mat kernel_dilate2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(src, src, kernel_dilate2);

    static cv::Mat kernel_erode2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(src, src, kernel_erode2);
}
// 在给定图像上寻找所有可能的灯条
bool findLightBolbsSJTU(Mat &input_img)
{
    LightBlobs light_blobs;
    cv::Mat color_channel;
    cv::Mat src_bin_light, src_bin_dim;
    std::vector<cv::Mat> channels;       // 通道拆分
     //Mat input_img = imread("aa.png");
    cv::split(input_img, channels);               /************************/
     color_channel = channels[2];        /************************/
    int light_threshold;
     light_threshold = 200;
    cv::threshold(color_channel, src_bin_light, light_threshold, 255, CV_THRESH_BINARY); // 二值化对应通道
    if (src_bin_light.empty()) return false;
    imagePreProcess(src_bin_light);                                  // 开闭运算

    cv::threshold(color_channel, src_bin_dim, 140, 255, CV_THRESH_BINARY); // 二值化对应通道
    if (src_bin_dim.empty()) return false;
    imagePreProcess(src_bin_dim); 

    // imshow("ee",src_bin_light);
    // imshow("e",src_bin_dim);
    // waitKey(0);

// 使用两个不同的二值化阈值同时进行灯条提取，减少环境光照对二值化这个操作的影响。
// 同时剔除重复的灯条，剔除冗余计算，即对两次找出来的灯条取交集。
    std::vector<std::vector<cv::Point>> light_contours_light, light_contours_dim;
    LightBlobs light_blobs_light, light_blobs_dim;
    std::vector<cv::Vec4i> hierarchy_light, hierarchy_dim;
    cv::findContours(src_bin_light, light_contours_light, hierarchy_light, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::findContours(src_bin_dim, light_contours_dim, hierarchy_dim, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    for (int i = 0; i < light_contours_light.size(); i++) {
        if (hierarchy_light[i][2] == -1) {
            cv::RotatedRect rect = cv::minAreaRect(light_contours_light[i]);
            if (isValidLightBlob(light_contours_light[i], rect)) {
                light_blobs_light.emplace_back(
                        rect, areaRatio(light_contours_light[i], rect), get_blob_color(input_img, rect)
                );
            }
        }
    }
    for (int i = 0; i < light_contours_dim.size(); i++) {
        if (hierarchy_dim[i][2] == -1) {
            cv::RotatedRect rect = cv::minAreaRect(light_contours_dim[i]);
            if (isValidLightBlob(light_contours_dim[i], rect)) {
                light_blobs_dim.emplace_back(
                        rect, areaRatio(light_contours_dim[i], rect), get_blob_color(input_img, rect)
                );
            }
        }
    }
    vector<int> light_to_remove, dim_to_remove;

    for (int l = 0; l != light_blobs_light.size(); l++) {
        for (int d = 0; d != light_blobs_dim.size(); d++) {
            if (isSameBlob(light_blobs_light[l], light_blobs_dim[d])) {
                if (light_blobs_light[l].area_ratio > light_blobs_dim[d].area_ratio) {
                    dim_to_remove.emplace_back(d);
                } else {
                    light_to_remove.emplace_back(l);
                }
            }
        }
    }
    sort(light_to_remove.begin(), light_to_remove.end(), [](int a, int b) { return a > b; });
    sort(dim_to_remove.begin(), dim_to_remove.end(), [](int a, int b) { return a > b; });
    for (auto x : light_to_remove) {
        light_blobs_light.erase(light_blobs_light.begin() + x);
    }
    for (auto x : dim_to_remove) {
        light_blobs_dim.erase(light_blobs_dim.begin() + x);
    }
    for (const auto &light : light_blobs_light) {
        light_blobs.emplace_back(light); 
     
    }
    for (const auto &dim : light_blobs_dim) {
        light_blobs.emplace_back(dim);
    }

    //showLightBlobs(input_img,"lightbolbs",light_blobs); 
    ArmorBoxes boxes;
    matchArmorBoxes(input_img,light_blobs,boxes);
    showArmorBoxes("res",input_img,boxes);
    return  0;
    
}
void showLightBlobs(const cv::Mat &input_image,string windows_name,const LightBlobs &light_blobs) {
            static Mat image2show;
            if (input_image.type() == CV_8UC1) { // 黑白图像
                cvtColor(input_image, image2show, COLOR_GRAY2RGB);
            } else if (input_image.type() == CV_8UC3) { //RGB 彩色
                image2show = input_image.clone();
            }

            for (const auto &light_blob:light_blobs) {
                Scalar color(0, 255, 0);
                
                cv::Point2f vertices[4];
                light_blob.rect.points(vertices);
                for (int j = 0; j < 4; j++) {
                    cv::line(image2show, vertices[j], vertices[(j + 1) % 4], color, 2);
                }
            }
            imshow(windows_name, image2show);
            waitKey(10);
    }

bool findLightBolbsCSDN(Mat &image){
    LightBlobs light_blobs;
    Mat image1,red_channel,diffimg,afterprc,light_loc;  /*创建图像容器*/         /*将项目中的before.png图像读入到image中*/
    vector<vector<Point> > contour;           /*定义二维浮点型变量存放找到的边界坐标*/
    bool bFlag = false;
    RotatedRect s;                            /*定义旋转矩形*/
    // vector<RotatedRect> vEllipse;             /*定以旋转矩形的向量，用于存储发现的目标区域*/
    // vector<RotatedRect> vRlt;
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
    threshold(red_channel,diffimg,20, 255, CV_THRESH_BINARY);               /*调用二值化函数得到二值图*/
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
                       light_blobs.emplace_back(
                                s, 
                                areaRatio(contour[i], s),
                                get_blob_color(image, s)
                            );                 //将发现的目标保存
                    }
                }

            }
    ArmorBoxes boxes;
    matchArmorBoxes(image,light_blobs,boxes);
    showArmorBoxes("res",image,boxes);
    return true;

}
