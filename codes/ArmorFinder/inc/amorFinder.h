#include<opencv2/opencv.hpp>
#include<string>
using namespace cv;
#define BLOB_RED    1
#define BLOB_BLUE   0
/**
 * @author:SJTU
 * @desc:灯条类，用于存储灯条信息
 */
class LightBlob {
public:
    RotatedRect rect;   //灯条位置
    double area_ratio;  //面积比
    double length;          //灯条长度
    uint8_t blob_color;      //灯条颜色

    LightBlob(cv::RotatedRect &r, double ratio, uint8_t color) : rect(r), area_ratio(ratio), blob_color(color) {
        length = max(rect.size.height, rect.size.width);
    };
    LightBlob() = default;

    double max(double x,double y){
        return (x>y)?(x):(y);
    };
};
typedef std::vector<LightBlob> LightBlobs;
/**
 *@author:SJTU
 *@desc:SJTU版本灯条查找过程中使用的工具函数
 */
class FinderTools{
public:
    //预处理，二值化后进行开闭运算
    static void imagePreProcess(Mat &src) {
        static cv::Mat kernel_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
        erode(src, src, kernel_erode);

        static cv::Mat kernel_dilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
        dilate(src, src, kernel_dilate);

        static cv::Mat kernel_dilate2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
        dilate(src, src, kernel_dilate2);

        static cv::Mat kernel_erode2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
        erode(src, src, kernel_erode2);
    }
    //轮廓面积和其最小外接矩形面积之比
    static double areaRatio(const std::vector<cv::Point> &contour, const cv::RotatedRect &rect) {
        return cv::contourArea(contour) / rect.size.area();
    }
    //旋转矩形的长宽比
    static double lw_rate(const cv::RotatedRect &rect) {
        return rect.size.height > rect.size.width ?
            rect.size.height / rect.size.width :
            rect.size.width / rect.size.height;
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
    //每个像素的每个通道的值都进行线性变换
    static void brightAdjust(Mat &src, Mat &dst, double dContrast, double dBright){
        int nVal;
        dst = Mat(src.size(), CV_8UC3);
        for (int nI = 0; nI<src.rows; nI++){
            Vec3b* p1 = src.ptr<Vec3b>(nI);
            Vec3b* p2 = dst.ptr<Vec3b>(nI);
            for (int nJ = 0; nJ <src.cols; nJ++){
                for (int nK = 0; nK < 3; nK++){
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
};
/**
 * @author:SJTU
 * @desc:装甲识别类，目前仅封装了灯条的初步识别方法
 */
class AmorFinder{
    private:
        Mat input_img,input_dim;
        Mat out_img,img_dim;
    public:
        AmorFinder(){
            std::cout<<"ArmorFinder constructed!"<<std::endl;
        };
        ~AmorFinder(){
            std::cout<<"ArmorFinder deleted!"<<std::endl;
        }
        void setImage(Mat img){
            this->input_img = img; 
        }
        void setImage(std::string path){
            this->input_img = cv::imread(path); 
        }
        /**
        *@author:王妍璐 江培玲
        *@func:preProcess(),preProcessV2()
        *@para:
        *      light_threshold:二值化阈值
        *@desc:输入图片预处理,包括亮度调节，开闭运算，二值化
        *      preProcess()对应findLightBlob()的预处理（STJU版）
        *      preProcessV2()对应findLightBlobV2()的预处理(V2版)
        */
        bool preProcess(int light_threshold){
            std::vector<cv::Mat> channels;
            split(input_img, channels); 
            threshold(channels[2],out_img,light_threshold,255,CV_THRESH_BINARY);
            FinderTools::imagePreProcess(out_img); 
            threshold(channels[2], img_dim, 140, 255, CV_THRESH_BINARY);
            FinderTools::imagePreProcess(img_dim);

            return true;

        }
        bool preProcessV2(int light_threshold){
            FinderTools::brightAdjust(input_img,input_dim,1,-120);
            std::vector<cv::Mat> channels;
            split(input_dim, channels); 
            threshold(channels[2], img_dim, light_threshold, 255, CV_THRESH_BINARY);
            FinderTools::imagePreProcess(img_dim);
            return true;
        }
        /**
         * @author:SJTU
         * @func:findLightBlob()
         * @para:None
         * @desc:
         *      预处理产生两张不同阈值下的二值图:out_img,img_dim.
         *      对两张二值图查找轮廓,求并集,初步筛选出所有可能灯条.
         *      最终查找到的灯条保存在LightBlobs类型的vector中，供后续分析.
         *      轮廓提取可参考下面帖子：
         *      https://blog.csdn.net/qq_30815237/article/details/86890067
         */ 
        LightBlobs findLightBlob(){
                std::vector<std::vector<cv::Point>> light_contours_light, light_contours_dim;//保存轮廓
                LightBlobs light_blobs_light, light_blobs_dim;//临时保存初筛的灯条
                std::vector<cv::Vec4i> hierarchy_light, hierarchy_dim;//保存轮廓嵌套关系
                cv::findContours(out_img, light_contours_light, hierarchy_light, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
                cv::findContours(img_dim, light_contours_dim, hierarchy_dim, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

                for (int i = 0; i < light_contours_light.size(); i++) {
                    if (hierarchy_light[i][2] == -1) {
                        cv::RotatedRect rect = cv::minAreaRect(light_contours_light[i]);
                        if (FinderTools::isValidLightBlob(light_contours_light[i], rect)) {
                            //vector
                            light_blobs_light.emplace_back(
                                    rect, FinderTools::areaRatio(light_contours_light[i], rect), FinderTools::get_blob_color(input_img, rect)
                            );
                        }
                    }
                }
                for (int i = 0; i < light_contours_dim.size(); i++) {
                    if (hierarchy_dim[i][2] == -1) {
                        cv::RotatedRect rect = cv::minAreaRect(light_contours_dim[i]);
                        if (FinderTools::isValidLightBlob(light_contours_dim[i], rect)) {
                            light_blobs_dim.emplace_back(
                               rect, FinderTools::areaRatio(light_contours_dim[i], rect), FinderTools::get_blob_color(input_img, rect)
                            );
                        }
                    }
                }

                //比较两个灯条类，是同一个则保留，面积更小的放入vector中，表示待移除
                std::vector<int> light_to_remove, dim_to_remove;
                for (int l = 0; l != light_blobs_light.size(); l++) {
                    for (int d = 0; d != light_blobs_dim.size(); d++) {
                        if (FinderTools::isSameBlob(light_blobs_light[l], light_blobs_dim[d])) {
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

                LightBlobs light_blobs;
                for (const auto &light : light_blobs_light) {
                    light_blobs.emplace_back(light);
                }
                for (const auto &dim : light_blobs_dim) {
                    light_blobs.emplace_back(dim);
                }
                return light_blobs;
        }
        /**
         * @author:代成浩组,王妍璐组
         * @func:findLightBlobV2()
         * @para:None
         * @desc:
         *      遍历轮廓，找出大于10个像素点的轮廓，拟合成旋转矩形
         *      判断该旋转矩形的中心5×55×5的像素块（从原图看）是否接近白色
         *      是则说明可能为装甲LED灯带所在区域，添加至light_blobs中，为下一步定位装甲做准备
         *      参考：https://blog.csdn.net/healingwounds/article/details/78583194
         */ 
        LightBlobs findLightBlobV2(){
            bool bFlag = false;
            LightBlobs light_blobs;
            std::vector<std::vector<cv::Point>> light_contours;
            cv::findContours(img_dim, light_contours, RETR_CCOMP , CHAIN_APPROX_SIMPLE);
            for (int i=0; i<light_contours.size(); i++){
                //判断当前轮廓是否大于10个像素点
                if (light_contours[i].size()> 10){  
                    //如果大于10个，则检测到目标区域   
                    //拟合目标区域成为椭圆，返回一个旋转矩形（中心、角度、尺寸）     
                    bFlag = true;                                                              
                    cv::RotatedRect rect = cv::fitEllipse(light_contours[i]);  
                    for (int nI = 0; nI < 5; nI++){
                        //遍历以旋转矩形中心点为中心的5*5的像素块
                        for (int nJ = 0; nJ < 5; nJ++){   
                            //判断该像素是否在有效的位置
                            if (rect.center.y - 2 + nJ > 0 
                                && rect.center.y - 2 + nJ < 480 
                                && rect.center.x - 2 + nI > 0 
                                && rect.center.x - 2 + nI <  640)  {   
                                //判断中心点是否接近白色
                                //获取遍历点点像素值
                                Vec3b v3b = input_img.at<Vec3b>((int)(rect.center.y - 2 + nJ), (int)(rect.center.x - 2 + nI));
                                if (v3b[0] < 200 || v3b[1] < 200 || v3b[2] < 200){
                                    //如果中心不是白色，则不是目标区域
                                    bFlag = false;
                                }
                             }
                        }
                    }
	                if (bFlag){
                           //将发现的目标保存
                           light_blobs.emplace_back(
                                rect, 
                                FinderTools::areaRatio(light_contours[i], rect),
                                FinderTools::get_blob_color(input_img, rect)
                            );          
                    }
                } 
            }
            return light_blobs;
        }
        /**
         * @author:STJU
         * @func:showLightBlobs()
         * @para:
         *       windows_name:窗口名
         *       light_blobs:存储灯条的vector
         * @desc:用于在原图上框处灯条
         */ 
        void showLightBlobs(std::string windows_name,const LightBlobs &light_blobs) {
            static Mat image2show;
            if (input_img.type() == CV_8UC1) { // 黑白图像
                cvtColor(input_img, image2show, COLOR_GRAY2RGB);
            } else if (input_img.type() == CV_8UC3) { //RGB 彩色
                image2show = input_img.clone();
            }

            for (const auto &light_blob:light_blobs) {
                Scalar color(0, 255, 0);
                if (light_blob.blob_color == BLOB_RED)
                    color = Scalar(0, 0, 255);
                else if (light_blob.blob_color == BLOB_BLUE)
                    color = Scalar(255, 0, 0);
                cv::Point2f vertices[4];
                light_blob.rect.points(vertices);
                for (int j = 0; j < 4; j++) {
                    cv::line(image2show, vertices[j], vertices[(j + 1) % 4], color, 2);
                }
            }
            imshow(windows_name, image2show);
            waitKey(0);
    }

};
