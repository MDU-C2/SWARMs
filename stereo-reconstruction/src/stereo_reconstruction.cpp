#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/CompressedImage.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>

#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/ChannelFloat32.h>
#include <geometry_msgs/Point32.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <dynamic_reconfigure/server.h>
#include <stereo_dense_reconstruction/CamToRobotCalibParamsConfig.h>
#include <fstream>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <std_msgs/Float32.h>
#include <string>
#include <sstream>
#include "opencv2/ximgproc/disparity_filter.hpp"

//Extra includes
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace cv::ximgproc;
using namespace std;

Mat img_left, img_right;
Mat XR, XT, Q, P1, P2;
Mat R1, R2, K1, K2, D1, D2, R;
Mat lmapx, lmapy, rmapx, rmapy;
Vec3d T;
stereo_dense_reconstruction::CamToRobotCalibParamsConfig config;
FileStorage calib_file;

#define scaling_factor 3

// Size of the calibration and output images can be changed
// Sizes need to be synced with the actual size of the calibration images and the size set for rectification
Size out_img_size(640, 480); // 320x240
// Size out_img_size(1920/scaling_factor, 1080/scaling_factor);
// Size calib_img_size(1920, 1080); // 640 x 480
Size calib_img_size(640, 480); // 640 x 480

// image_transport::Publisher dmap_pub;
image_transport::Publisher dmap_filtered_pub;
ros::Publisher pcl_pub;
ros::Publisher dist_pub;

typedef pcl::PointCloud<pcl::PointXYZRGB> PointCloud;

long unsigned int pcl_time = 0;

double fMaxDistance;

Mat composeRotationCamToRobot(float x, float y, float z) {
  Mat X = Mat::eye(3, 3, CV_64FC1);
  Mat Y = Mat::eye(3, 3, CV_64FC1);
  Mat Z = Mat::eye(3, 3, CV_64FC1);
  
  X.at<double>(1,1) = cos(x);
  X.at<double>(1,2) = -sin(x);
  X.at<double>(2,1) = sin(x);
  X.at<double>(2,2) = cos(x);

  Y.at<double>(0,0) = cos(y);
  Y.at<double>(0,2) = sin(y);
  Y.at<double>(2,0) = -sin(y);
  Y.at<double>(2,2) = cos(y);

  Z.at<double>(0,0) = cos(z);
  Z.at<double>(0,1) = -sin(z);
  Z.at<double>(1,0) = sin(z);
  Z.at<double>(1,1) = cos(z);
  
  return Z*Y*X;
} 

Mat composeTranslationCamToRobot(float x, float y, float z) {
  return (Mat_<double>(3,1) << x, y, z);
}

void debugCamToRobotTransformation(Mat& R, Mat& T) {
  R = composeRotationCamToRobot(config.PHI_X,config.PHI_Y,config.PHI_Z);
  T = composeTranslationCamToRobot(config.TRANS_X,config.TRANS_Y,config.TRANS_Z);

}

void publishPointCloud(Mat& img_left, Mat& dmap) {
  debugCamToRobotTransformation(XR, XT);
  Mat V = Mat(4, 1, CV_64FC1);
  Mat pos = Mat(4, 1, CV_64FC1);
  vector< pcl::PointXYZRGB > points;
  PointCloud pc;
  pc.header.frame_id = "base_link";
  pc.header.stamp = ros::Time::now().toSec();//pcl_time;
  pcl_time++;
  for (int i = 0; i < img_left.cols; i++) {
    for (int j = 0; j < img_left.rows; j++) {
      int d = dmap.at<uchar>(j,i);
      // if low disparity, then ignore
      if (d < 2) {
        continue;
      }

      // V is the vector to be multiplied to Q to get
      // the 3D homogenous coordinates of the image point
      V.at<double>(0,0) = (double)(i);
      V.at<double>(1,0) = (double)(j);
      V.at<double>(2,0) = (double)d;
      V.at<double>(3,0) = 1.;
      pos = Q * V; // 3D homogeneous coordinate
      double X = pos.at<double>(0,0) / pos.at<double>(3,0);
      double Y = pos.at<double>(1,0) / pos.at<double>(3,0);
      double Z = pos.at<double>(2,0) / pos.at<double>(3,0);
      Mat point3d_cam = Mat(3, 1, CV_64FC1);
      point3d_cam.at<double>(0,0) = X;
      point3d_cam.at<double>(1,0) = Y;
      point3d_cam.at<double>(2,0) = Z;

      // transform 3D point from camera frame to robot frame
      Mat point3d_robot = XR * point3d_cam + XT;

      pcl::PointXYZRGB pt;
      pt.x = point3d_robot.at<double>(0,0);
      pt.y = point3d_robot.at<double>(1,0);
      pt.z = point3d_robot.at<double>(2,0);

      int32_t red, blue, green;
      red = img_left.at<Vec3b>(j,i)[2];
      green = img_left.at<Vec3b>(j,i)[1];
      blue = img_left.at<Vec3b>(j,i)[0];
      int32_t rgb = (red << 16 | green << 8 | blue);
      pt.rgb = *reinterpret_cast<float*>(&rgb);

      pc.points.push_back(pt);

    }
  }
  pcl_pub.publish(pc);
}


Mat generateDisparityMap(Mat& left, Mat& right, std_msgs::Header header) {
  if (left.empty() || right.empty()) 
    return left;

  int ndisparities = 80;//32*5;   /**< Range of disparity */
  int SADWindowSize = 21;  /* *< Size of the block window. Must be odd  21*3 */ 
  double minVal; double maxVal;

	Mat left_disp, right_disp;
	Mat filtered_disp;

	Rect ROI;
	Ptr<DisparityWLSFilter> wls_filter;

  //! [matching]
  Ptr<StereoBM> left_matcher = StereoBM::create( ndisparities, SADWindowSize );
  wls_filter = createDisparityWLSFilter(left_matcher);
  Ptr<StereoMatcher> right_matcher = createRightMatcher(left_matcher);

  left_matcher->compute(left, right,left_disp);
  right_matcher->compute(right,left, right_disp);
  //! [matching]

  //! [filtering]
  wls_filter->setLambda(30000.0);
  wls_filter->setSigmaColor(3.1);
  wls_filter->filter(left_disp,left,filtered_disp,right_disp);
  //! [filtering]

//   // Get the ROI that was used in the last filter call:
  ROI = wls_filter->getROI();

  // Downsampling of the disparity map to 8 bits
  Mat show_filtered = Mat( left.rows, left.cols, CV_8UC1, Scalar(0) );
  filtered_disp.convertTo( show_filtered, CV_8U , 0.1);
  minMaxLoc( show_filtered, &minVal, &maxVal );
  
  // Calculate the distance of the closest object to the camera
  fMaxDistance = (1. / Q.at<double>(3, 2)) * Q.at<double>(2, 3);
  double fDisparity = maxVal / (double)StereoMatcher::DISP_SCALE;
  double dist = fMaxDistance / (fDisparity * 10); 

  std_msgs::Float32 dist_msg;
  dist_msg.data = dist;
  dist_pub.publish(dist_msg);


  if (!show_filtered.empty()) {
    if(false) // Change to true for saving of the disparity map
    {
      ostringstream convert;
      convert << header.stamp.sec;
      string disp_filename = "/home/odroid/Documents/cpp/pcds/";
      disp_filename += convert.str();
      convert << header.stamp.nsec;
      disp_filename += "_" + convert.str();
      disp_filename += ".png";
      imwrite(disp_filename, show_filtered);
    }
    // Publishing the disparity map
    sensor_msgs::ImagePtr disp_filt_msg;
    disp_filt_msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", show_filtered).toImageMsg();
    dmap_filtered_pub.publish(disp_filt_msg);
  }
  return show_filtered;

}

void imgCallback(const sensor_msgs::CompressedImageConstPtr& msg_left, const 
sensor_msgs::CompressedImageConstPtr& msg_right) {
  Mat img_left_color = cv::imdecode(cv::Mat(msg_left->data), CV_LOAD_IMAGE_COLOR);
  Mat img_left = cv::imdecode(cv::Mat(msg_left->data), CV_LOAD_IMAGE_GRAYSCALE);
  Mat img_right = cv::imdecode(cv::Mat(msg_right->data), CV_LOAD_IMAGE_GRAYSCALE);
  if (img_left.empty() || img_right.empty())
    return;
  std_msgs::Header header = msg_left->header;

  Mat dmap = generateDisparityMap(img_left, img_right, header);

  // Pointcloud publishing increases execution time
  // publishPointCloud(img_left_color, dmap);
}

void findRectificationMap(FileStorage& calib_file, Size finalSize) {
  Rect validRoi[2];
  cout << "starting rectification" << endl;
  stereoRectify(K1, D1, K2, D2, calib_img_size, R, Mat(T), R1, R2, P1, P2, Q, 
                CV_CALIB_ZERO_DISPARITY, 0, finalSize, &validRoi[0], &validRoi[1]);
  cout << "done rectification" << endl;
}

void paramsCallback(stereo_dense_reconstruction::CamToRobotCalibParamsConfig 
&conf, uint32_t level) {
  config = conf;
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "stereo_reconstruction");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::ImageTransport it2(nh);
  
  calib_file = FileStorage(argv[1], FileStorage::READ);
  calib_file["K1"] >> K1;
  calib_file["K2"] >> K2;
  calib_file["D1"] >> D1;
  calib_file["D2"] >> D2;
  calib_file["R"] >> R;
  calib_file["T"] >> T;
  calib_file["XR"] >> XR;
  calib_file["XT"] >> XT;
  findRectificationMap(calib_file, out_img_size);

  
  message_filters::Subscriber<sensor_msgs::CompressedImage> sub_img_left(nh, 
"/camera_left_rect/image_color/compressed", 100);
  message_filters::Subscriber<sensor_msgs::CompressedImage> sub_img_right(nh, 
"/camera_right_rect/image_color/compressed", 100);
  
  typedef 
message_filters::sync_policies::ApproximateTime<sensor_msgs::CompressedImage, 
sensor_msgs::CompressedImage> SyncPolicy;
  message_filters::Synchronizer<SyncPolicy> sync(SyncPolicy(10), 
sub_img_left, sub_img_right);
  sync.registerCallback(boost::bind(&imgCallback, _1, _2));
  
  dynamic_reconfigure::Server<stereo_dense_reconstruction::
CamToRobotCalibParamsConfig> server;
  dynamic_reconfigure::Server<stereo_dense_reconstruction::
CamToRobotCalibParamsConfig>::CallbackType f;

  f = boost::bind(&paramsCallback, _1, _2);
  server.setCallback(f);
  
  dmap_filtered_pub = it2.advertise("/camera_left_rect/disparity_map_filtered", 1);
  pcl_pub = 
nh.advertise<PointCloud>("/camera_left_rect/point_cloud",100);

  dist_pub = nh.advertise<std_msgs::Float32>("/minimal_distance", 100);

  ros::spin();
  return 0;
}
