#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "math.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


void call_back(const sensor_msgs::LaserScan::ConstPtr& msg){
    int n=(msg->angle_max-msg->angle_min)/msg->angle_increment;//calcolo il numero di scansioni laser
    float min_range=msg->ranges[0];
    float x_min;
    float y_min;
    int min=0;
    cv::Point coordinate[n];//vettore contenente x e y per ogni scansione laser
    cv::Mat img(600, 800, CV_8UC3, cv::Scalar::all(0));//creo matrice
    for(int i=0;i<n;i++){//calcolo il numero min-esimo riferito alla scansione minore e scrittura di x e y e tutte le altre coordinate
		if(min_range>msg->ranges[i]){
			min_range=msg->ranges[i];
			min=i;
			x_min=2*((-13*(msg->ranges[i]*cos(i*(msg->angle_increment)+msg->angle_min)))+300);
			y_min=2*((13*(msg->ranges[i]*sin(i*(msg->angle_increment)+msg->angle_min)))+400);
		}
		coordinate[i]=cv::Point((-13*(msg->ranges[i]*cos(i*(msg->angle_increment)+msg->angle_min)))+300,(13*(msg->ranges[i]*sin(i*(msg->angle_increment)+msg->angle_min)))+400);
		
		
	}
	
	for(int j=0;j<n-1;j++){
		line(img,coordinate[j],coordinate[j+1],cv::Scalar(110,220,0),1,8);
		if(j==min)circle(img,cv::Point(x_min,y_min),11,cv::Scalar(255,0,0),1,8,1);
			}
        
        cv::imshow("example",img);
        cv::waitKey(30);
	

	
	
	
}

int main(int argc,char** argv){
	
	ros::init(argc,argv,"dist_laser");
	ros::NodeHandle l;
	
	ros::Subscriber sub=l.subscribe("base_scan",1000,call_back);
    
    
	ros::spin();
	return 0;
}
