#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/LaserScan.h>
#include <stdio.h>
#include <stdbool.h>

int err;

float   range_ahead;
float	range_left;
float	range_right;
int state = 0;
bool timer = false;
bool front_obj = false;

void img_cb(const sensor_msgs::ImageConstPtr& msg) {
	cv_bridge::CvImagePtr   cv_ptr;
	cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

	int h = cv_ptr->image.rows;
	int w = cv_ptr->image.cols;

	cv::Mat hsv_img;
	cvtColor(cv_ptr->image, hsv_img, CV_BGR2HSV);

	cv::Mat mask_white_left;
	cv::Mat mask_white_right;
	cv::Mat mask_yellow;

	cv::Scalar lower_white = cv::Scalar(0,0,175);
	cv::Scalar upper_white = cv::Scalar(255,255,255);
	inRange(hsv_img, lower_white, upper_white, mask_white_left);
	inRange(hsv_img, lower_white, upper_white, mask_white_right);
	
	cv::Scalar lower_yellow = cv::Scalar(20, 100, 100);
	cv::Scalar upper_yellow = cv::Scalar(40,255,255);
	inRange(hsv_img, lower_yellow, upper_yellow, mask_yellow);

	//int search_top = 1 * h/2;
	//int search_bot = 3 * h/4 + 30;

	int search_left_max = 1 * w/1.2;
	int search_left_bot = 1 * h/2 + 25;

	int search_right_min = 1 * w/2 - 20;
	int search_right_bot = 1 * h/2 + 25;

	int search_top = 2 * h/3;
	int search_bot = 2 * h/3 +25;

	for(int y=0;y<h;y++){
		for(int x=0;x<w;x++){
			if(y<search_left_bot) mask_white_left.at<uchar>(y,x) = 0;
			if(x>=search_left_max) mask_white_left.at<uchar>(y,x) = 0;

			if(y<search_right_bot) mask_white_right.at<uchar>(y,x) = 0;
			if(x<=search_right_min) mask_white_right.at<uchar>(y,x) = 0;

			if(y<search_top) mask_yellow.at<uchar>(y,x) = 0;
			if(y>=search_bot) mask_yellow.at<uchar>(y,x) = 0;
		}
	}
	cv::Moments m = moments(mask_white_left);
	cv::Moments m1 = moments(mask_white_right);
	cv::Moments m2 = moments(mask_yellow);

	if(m.m00 > 0) {
		int cx = m.m10 / m.m00;
		int cy = m.m01 / m.m00;

		cv::circle(cv_ptr ->image, cv::Point(cx, cy), 20, CV_RGB(255,0,0), -1);
		if(front_obj == true)
		{
		err = cx -w/2;
		}
	}
	if(m1.m00 > 0) {
		int cx1 = m1.m10 / m1.m00;
		int cy1 = m1.m01 / m1.m00;

		cv::circle(cv_ptr ->image, cv::Point(cx1, cy1), 20, CV_RGB(255,0,0), -1);
		
	}
	if(m2.m00 > 0) {
		int cx2 = m2.m10 / m2.m00;
		int cy2 = m2.m01 / m2.m00;

		cv::circle(cv_ptr ->image, cv::Point(cx2, cy2), 20, CV_RGB(77,0,0), -1);
		if(front_obj == false)
		{
		err = cx2 -w/2;
		}
	}

	
	imshow("road window", cv_ptr->image);
	//imshow("road window", mask_white_left);
	//imshow("road window", mask_white_right);
	cv::waitKey(3);
}

int scan_cb(const sensor_msgs::LaserScan::ConstPtr& msg) {
	
		range_ahead = msg->ranges[0];
		range_right = msg->ranges[345];
		range_left = msg->ranges[15];
		printf("range_right : %f_____range_left : %f_____range_ahead : %f\n", range_right,range_left,range_ahead);
			if(range_right < 1.0){
				if(range_right < range_left){
				front_obj = true;
				timer = true;
			 	return 0;}
				else if(range_right > range_left){
				front_obj = true;
				timer = true;
				return 0;}
			}
			else if(range_left < 1.0){
				if(range_left < range_right){
				front_obj = true;
				timer =true;
				return 0;}
				else if(range_left > range_right){
				front_obj = true;
				timer = true;
				return 0;}
			}
			
				


}

int main(int argc, char **argv) {
	ros::init(argc, argv, "followbot");
	ros::NodeHandle n;
	image_transport::ImageTransport it(n);
	ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	//image_transport::Subscriber img_sub= it.subscribe("raspicam_node/image", 1, img_cb);
	image_transport::Subscriber img_sub = it.subscribe("camera/rgb/image_raw",1,img_cb);
	
	ros::Subscriber scan_sub = n.subscribe<sensor_msgs::LaserScan>("scan",1,scan_cb);
	
	cv::namedWindow("road window");
	ros::Rate   loop_rate(30);
	geometry_msgs::Twist cmd;
	ros::Time state_start;

	cmd.linear.x = 0.15;
	

	while(ros::ok()) {
		if(timer == true)
		{
	 		state_start = ros::Time::now();
	 		timer = false;
		}
		if(front_obj == false)
		{	
			cmd.angular.z = -err/300.;
		}
		else if(front_obj == true)
		{
			cmd.angular.z = -err/900.;
			if(ros::Time::now() - state_start > ros::Duration(9.0))
			{
				front_obj = false;	
			}
	}
	
	cmd_pub.publish(cmd);
	ros::spinOnce();
	loop_rate.sleep();
	}
}
