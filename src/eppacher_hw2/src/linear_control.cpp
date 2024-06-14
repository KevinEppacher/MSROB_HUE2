#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include <sstream>
#include <dynamic_reconfigure/server.h>
#include <eppacher_hw2/dynamic_reconfigure_paramConfig.h>
#include "robot.h"


double x_G=10.0, y_G=10.0;
int theta_G=50.0*M_PI/180;
double x_R, y_R, theta_R;
double x_R_Gesch;
double k_rho=0.2;
double k_alpha=0.3;
double k_beta=0.3;
double kd=0.02;
double theta_R_Geschw;
bool on_off_switch;

double current_rho;
double last_rho;

void callback(eppacher_hw2::dynamic_reconfigure_paramConfig &config, uint32_t level) 
{
 ROS_INFO("Reconfigure Request: %lf %lf %d %lf %lf %lf %d",
            x_G=config.x_param,
            y_G=config.y_param,
            theta_G=(config.theta_goal_param)*M_PI/180,
            k_alpha=config.rho_param,
            k_beta=config.alpha_param,
            k_rho=config.beta_param,
            on_off_switch=config.turn_on_turn_off);

}

void counterCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    x_R=msg->pose.pose.position.x;
    y_R=msg->pose.pose.position.y;
    theta_R=msg->pose.pose.orientation.z;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "linear_control_publisher"); //type
    ros::NodeHandle n;
    ros::Publisher linera_control_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 50);
    ros::Subscriber feedback_pos=n.subscribe("odom",1000,counterCallback);
    dynamic_reconfigure::Server<eppacher_hw2::dynamic_reconfigure_paramConfig> server;
    ros::Rate loop_rate(1);
    geometry_msgs::Twist cmd_msg;
    dynamic_reconfigure::Server<eppacher_hw2::dynamic_reconfigure_paramConfig>::CallbackType f;
    f = boost::bind(&callback, _1, _2);
    server.setCallback(f);

    robot burger;

    while (ros::ok())
    {
        burger.linear_control( x_R,  y_R,  theta_R,  x_G,  y_G,  theta_G,  k_alpha,  k_beta, k_rho,  on_off_switch, current_rho, last_rho);
        ROS_INFO("                                                                         ");
        ROS_INFO("rho %lf alpha %lf beta %lf",burger.rho,burger.alpha,burger.beta);
        ROS_INFO("v %lf w %lf",cmd_msg.linear.x,cmd_msg.angular.z);
        ROS_INFO("x_G %lf y_G %lf",burger.x_G, burger.y_G);
        ROS_INFO("x_R %lf y_R %lf",x_R, y_R);
        ROS_INFO("x_G %lf y_G %lf",x_G, y_G);
        ROS_INFO("                                                                         ");

     
        cmd_msg.linear.x=burger.x_R_Gesch;
        cmd_msg.angular.z=burger.theta_R_Geschw;
        linera_control_pub.publish(cmd_msg);
        last_rho=burger.current_rho;

        ros::spinOnce();
        loop_rate.sleep();

    }

  return 0;
}