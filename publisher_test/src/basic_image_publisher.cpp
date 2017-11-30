#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher left_pub = it.advertise("camera_left/image_color", 1);
  image_transport::Publisher right_pub = it.advertise("camera_right/image_color", 1);

  ros::Rate loop_rate(5);
  while (nh.ok()) {
    cv::Mat left_image = cv::imread("/root/catkin_ws/src/SWARMs/publisher_test/src/scene_l.jpg", CV_LOAD_IMAGE_COLOR);
    //cv::waitKey(30);
    cv::Mat right_image = cv::imread("/root/catkin_ws/src/SWARMs/publisher_test/src/scene_r.jpg", CV_LOAD_IMAGE_COLOR);
    //cv::waitKey(30);
    if(!left_image.empty() && !right_image.empty()){

      sensor_msgs::ImagePtr left_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", left_image).toImageMsg();
      sensor_msgs::ImagePtr right_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", right_image).toImageMsg();

      left_pub.publish(left_msg);
      right_pub.publish(right_msg);
      system("cd /root/catkin_ws/src/SWARMs/publisher_test/src/ && rm *.jpg");
      ROS_INFO("Publish");
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
}
