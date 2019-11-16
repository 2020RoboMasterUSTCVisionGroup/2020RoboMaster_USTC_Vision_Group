#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
using namespace cv;
class VideoWarp{
    public:
        VideoCapture capture;
    public:
        void videoInit(std::string file_path){
             capture=VideoCapture(file_path);
             if (!capture.isOpened()){
                 std::cout << "Vidoe open failed!" << std::endl;
             }
        }
        Mat getNext(){
            Mat frame;
            capture>>frame;
            return frame;
        }
};