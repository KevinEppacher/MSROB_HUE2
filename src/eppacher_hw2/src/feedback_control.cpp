#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

void counterCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    //ROS_INFO("%s", msg->header.frame_id.c_str());
    //ROS_INFO("%f", msg->twist.twist.linear.x);
    //ROS_INFO("POS X: %f", msg->pose.pose.position.x);
    //ROS_INFO("Pos Y: %f", msg->pose.pose.position.y);
    //ROS_INFO("linear_X: %f", msg->twist.twist.linear.x);
    //ROS_INFO("linear_Y: %f", msg->twist.twist.linear.y);
    //ROS_INFO("angular_z: %f", msg->twist.twist.angular.z);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "sec_sub_node");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("odom", 1000, counterCallback);
    ros::spin();

    return 0;
}
