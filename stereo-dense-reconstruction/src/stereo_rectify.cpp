#include <ros/ros.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <dynamic_reconfigure/server.h>
#include <ctime>
#include <vector>

using namespace std;
using namespace cv;

Mat XR, XT, Q, P1, P2;
Mat R1, R2, K1, K2, D1, D2, R;
//float D_left[5], D_right[5];
Vec3d T;
Mat lmapx, lmapy, rmapx, rmapy;
FileStorage calib_file;
// Size out_img_size(768, 432); //kitti - 489x180, 1392x512 320x240

#define scaling_factor 3

Size out_img_size(640 , 480); // 320x240
//Size out_img_size(1920/scaling_factor, 1080/scaling_factor);
//Size calib_img_size(1920, 1080); // 640 x 480
Size calib_img_size(640, 480 ); // 640 x 480

sensor_msgs::CameraInfo ci_left;
sensor_msgs::CameraInfo ci_right;

image_transport::Publisher pub_img_left;
image_transport::Publisher pub_img_right;

ros::Publisher pub_info_left;
ros::Publisher pub_info_right;

void undistortRectifyImage(Mat& src, Mat& dst, FileStorage& calib_file, int 
left = 1) {
  if (left == 1) {
    remap(src, dst, lmapx, lmapy, cv::INTER_LINEAR);
  } else {
    remap(src, dst, rmapx, rmapy, cv::INTER_LINEAR);
  }
}

void findRectificationMap(FileStorage& calib_file, Size finalSize) {
  Rect validRoi[2];
  cout << "starting rectification" << endl;
  stereoRectify(K1, D1, K2, D2, calib_img_size, R, Mat(T), R1, R2, P1, P2, Q, 
                CV_CALIB_ZERO_DISPARITY, 0, finalSize, &validRoi[0], 
&validRoi[1]);
  cout << "done rectification" << endl;
  calib_file["R1"] >> R1;
  cv::initUndistortRectifyMap(K1, D1, R1, P1, finalSize, CV_32F, lmapx, 
lmapy);
  cv::initUndistortRectifyMap(K2, D2, R2, P2, finalSize, CV_32F, rmapx, 
rmapy);
}

void imgLeftCallback(const sensor_msgs::ImageConstPtr& msg) {
  try
  {

     if(ci_left.header.stamp == ci_right.header.stamp){
      ci_left.header.stamp = ros::Time::now();
     }
     else{
       ci_left.header.stamp = ci_right.header.stamp;
     }
    pub_info_left.publish(ci_left);

    Mat tmp = cv_bridge::toCvShare(msg, "bgr8")->image;
    //Mat tmp = cv::imdecode(cv::Mat(msg->data), CV_LOAD_IMAGE_COLOR);
    if (tmp.empty()) return;
    Mat dst;
    undistortRectifyImage(tmp, dst, calib_file, 1);
    sensor_msgs::ImagePtr img_left;
    img_left = cv_bridge::CvImage(msg->header, "bgr8", 
dst).toImageMsg();
    
    img_left->header.stamp = ci_left.header.stamp;

    pub_img_left.publish(img_left);

  }
  catch (cv_bridge::Exception& e)
  {
  }
}

void imgRightCallback(const sensor_msgs::ImageConstPtr& msg) {
  try
  {
     if(ci_right.header.stamp == ci_left.header.stamp){
      ci_right.header.stamp = ros::Time::now();
     }
     else{
       ci_right.header.stamp = ci_left.header.stamp;
     }
    pub_info_right.publish(ci_right);

    Mat tmp = cv_bridge::toCvShare(msg, "bgr8")->image;
    //Mat tmp = cv::imdecode(cv::Mat(msg->data), CV_LOAD_IMAGE_COLOR);
    if (tmp.empty()) return;
    Mat dst;
    undistortRectifyImage(tmp, dst, calib_file, 0);
    sensor_msgs::ImagePtr img_right;
    img_right = cv_bridge::CvImage(msg->header, "bgr8", 
dst).toImageMsg();
    img_right->header.stamp = ci_right.header.stamp;
    pub_img_right.publish(img_right);

    
  }
  catch (cv_bridge::Exception& e)
  {
  }
}

void initCameraInfo(){
  ci_left.header.frame_id = "base_link";
  ci_left.height = out_img_size.height;
  ci_left.width = out_img_size.width;
  ci_left.distortion_model = "plumb_bob";

  ci_right.header.frame_id = "base_link";
  ci_right.height = out_img_size.height;
  ci_right.width = out_img_size.width;
  ci_right.distortion_model = "plumb_bob";
  
  //ci_left.D = new(float[5]);
  int iterator = 0;
  for(int i = 0; i < D1.rows; i++){
    for(int j = 0; j < D1.cols; j++){
      ci_left.D.push_back(D1.at<double>(i,j));
      ci_right.D.push_back(D2.at<double>(i,j));
    }
  }

  iterator = 0;
  for(int i = 0; i < K1.rows; i++){
    for(int j = 0; j < K1.cols; j++){
      ci_left.K[iterator] = K1.at<double>(i,j);
      ci_right.K[iterator++] = K2.at<double>(i,j);
    }
  }
  iterator = 0;
  for(int i = 0; i < R1.rows; i++){
    for(int j = 0; j < R1.cols; j++){
      ci_left.R[iterator] = R1.at<double>(i,j);
      ci_right.R[iterator++] = R2.at<double>(i,j);
    }
  }
  iterator = 0;
  for(int i = 0; i < P1.rows; i++){
    for(int j = 0; j < P1.cols; j++){
      ci_left.P[iterator] = P1.at<double>(i,j);
      ci_right.P[iterator++] = P2.at<double>(i,j);
    }
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "stereo_rectify");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  
  calib_file = FileStorage(argv[1], FileStorage::READ);
  calib_file["K1"] >> K1;
  calib_file["K2"] >> K2;
  calib_file["D1"] >> D1;
  calib_file["D2"] >> D2;
  calib_file["R"] >> R;
  calib_file["T"] >> T;
  findRectificationMap(calib_file, out_img_size);

  initCameraInfo();

  ros::Subscriber sub_img_left = nh.subscribe("camera_left/image_color", 1, imgLeftCallback);
  ros::Subscriber sub_img_right = nh.subscribe("camera_right/image_color", 1, imgRightCallback);

  pub_img_left = it.advertise("/camera_left_rect/image_color", 1);
  pub_img_right = it.advertise("/camera_right_rect/image_color", 1);

  pub_info_left = nh.advertise<sensor_msgs::CameraInfo>("/camera_left_rect/camera_info", 1);
  pub_info_right = nh.advertise<sensor_msgs::CameraInfo>("/camera_right_rect/camera_info", 1); 
   
  ros::spin();
  return 0;
}
