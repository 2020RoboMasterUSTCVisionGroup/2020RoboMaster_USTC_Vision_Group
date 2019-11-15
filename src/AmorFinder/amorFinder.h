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
 *@desc:灯条查找过程中使用的工具函数
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
};
/**
 * @author:SJTU
 * @desc:装甲识别类，目前仅封装了灯条的初步识别方法
 */
class AmorFinder{
    private:
        Mat input_img;
        Mat out_img,img_dim;
    public:
        AmorFinder(Mat input_img){
            this->input_img = input_img;
        };
        AmorFinder(std::string path){
            this->input_img = imread(path);
        };
        ~AmorFinder(){
            std::cout<<"ArmorFinder delete!"<<std::endl;
        }
        //预处理，得到开闭运算后的图片
        bool preProcess(int light_threshold){
            std::vector<cv::Mat> channels;
            split(input_img, channels); 

            threshold(channels[2],out_img,light_threshold,255,CV_THRESH_BINARY);
            FinderTools::imagePreProcess(out_img); 
    
            threshold(channels[2], img_dim, 140, 255, CV_THRESH_BINARY);
            FinderTools::imagePreProcess(out_img);

            return true;

        }
        /**
         * func:findLightBlob()
         * para:None
         * desc:预处理产生两张不同阈值下的二值图:out_img,img_dim.
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
