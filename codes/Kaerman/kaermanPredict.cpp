#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
using namespace cv;
using namespace std;
Point kaermanPredict(Point &input);
void KF(Point input);
class Kalman{
	private:
		const int stateNum=4;                                      //状态值4×1向量(x,y,△x,△y)
	    const int measureNum=2;                                    //测量值2×1向量(x,y)	
		KalmanFilter *KF ;	
		Mat prediction;
		Mat measurement ;  
		Point predict_pt ;
	public:
        Kalman(){
			this->KF=new KalmanFilter(stateNum, measureNum, 0);
			this->KF->transitionMatrix = (Mat_<float>(4, 4) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);  //转移矩阵A
			

			setIdentity(KF->measurementMatrix);                                             //测量矩阵H
			setIdentity(KF->processNoiseCov, Scalar::all(1e-5));                            //系统噪声方差矩阵Q
			setIdentity(KF->measurementNoiseCov, Scalar::all(1e-1));                        //测量噪声方差矩阵R
			setIdentity(KF->errorCovPost, Scalar::all(1));                                  //后验错误估计协方差矩阵P
			//rng.fill(K input(200,300);ePost,RNG::UNIFORM,0,winHeight>winWidth?winWidth:winHeight);   //初始状态值x(0)
			randn(KF->statePost, Scalar::all(0), Scalar::all(0.1));
		}
		void predict(Point input){
		
		    this->measurement=Mat::zeros(measureNum, 1, CV_32F); 
			this->prediction=this->KF->predict();
			this->predict_pt= Point(this->prediction.at<float>(0),this->prediction.at<float>(1) );   //预测值(x',y')
			//3.update measurement
			measurement.at<float>(0) = (float)input.x;
			measurement.at<float>(1) = (float)input.y;		
			//4.update
			KF->correct(measurement);

			cout<<"point:"<<input.x<<","<<input.y<<endl;
			cout<<"predit_point:"<<predict_pt.x<<","<<predict_pt.y<<endl;
		}

};
      
		


		//2.kalman prediction

 int main()
//Point kaermanPredict(Point &input)
{
    Point  input(200,300);
	Kalman *kf;  
	kf=new Kalman();
    int c=100;
	while(c){
		input.x=200-c;
		input.y=300-c;
		kf->predict(input);
		c--;
		
	}
	return 0;
 }
	//1.kalman filter setup
	//RNG rng;

	 
		//draw 
	// 	Mat image(winHeight,winWidth,CV_8UC3,Scalar(0));
	//  image.setTo(Scalar(255,255,255,0));
	//  circle(image,predict_pt,5,Scalar(0,255,0),3);    //predicted point with green
	//  circle(image,input,5,Scalar(255,0,0),3); //current position with red		
		
	//  char buf[256];
	//  sprintf(buf,"predicted position:(%3d,%3d)",predict_pt.x,predict_pt.y);
	//  putText(image,buf,Point(10,30),CV_FONT_HERSHEY_SCRIPT_COMPLEX,1,Scalar(0,0,0),1,8);
	//  sprintf(buf,"current position :(%3d,%3d)",input.x,input.y);
	//  putText(image,buf,cvPoint(10,60),CV_FONT_HERSHEY_SCRIPT_COMPLEX,1,Scalar(0,0,0),1,8);
		
	// 	imshow("kalman", image);
	// 	int key=waitKey(3);
	// 	if (key==27){
	// 		break;   
	// 	}		
		
   // return(predict_pt.x);
//}
// int main()
// {
// 	Point input(200,300);
// 	const int stateNum=4;                                      //状态值4×1向量(x,y,△x,△y)
// 	const int measureNum=2;                                    //测量值2×1向量(x,y)	
// 	KalmanFilter KF(stateNum, measureNum, 0);	
// 	KF.transitionMatrix = (Mat_<float>(4, 4) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);  //转移矩阵A
// 		setIdentity(KF.measurementMatrix);                                             //测量矩阵H
// 			setIdentity(KF.processNoiseCov, Scalar::all(1e-5));                            //系统噪声方差矩阵Q
// 			setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));                        //测量噪声方差矩阵R
// 			setIdentity(KF.errorCovPost, Scalar::all(1));       
// 			randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));

// 	int c=100;
// 	while(c)
// 	{
			
// 		        this->predict_pt= Point(this->prediction.at<float>(0),this->prediction.at<float>(1) );   //预测值(x',y')                   //后验错误估计协方差矩阵P
// 		//rng.fill(K input(200,300);ePost,RNG::UNIFORM,0,winHeight>winWidth?winWidth:winHeight);   //初始状态值x(0)
		
// 	        //初始测量值x'(0)，因为后面要更新这个值，所以必须先定义
// 		Mat measurement = Mat::zeros(measureNum, 1, CV_32F);     
// 		input.x=200-c;
//     	input.y=300-c;
// 		Mat prediction = KF.predict();
// 		Point predict_pt = Point(prediction.at<float>(0),prediction.at<float>(1) );   //预测值(x',y')
// 		//3.update measurement
// 		measurement.at<float>(0) = (float)input.x;
// 		measurement.at<float>(1) = (float)input.y;		
 
// 		//4.update
// 		KF.correct(measurement);
// 		c--;
//      	cout<<"point:"<<input.x<<","<<input.y<<endl;
//    		cout<<"predit_point:"<<predict_pt.x<<","<<predict_pt.y<<endl;
// 	}
// }