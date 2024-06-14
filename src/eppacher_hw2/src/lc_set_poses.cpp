#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include <sstream>
#include <dynamic_reconfigure/server.h>
#include <array>
#include <vector>
#include <eppacher_hw2/dynamic_reconfigure_param_2Config.h>
#include "robot.h"

using namespace std;

///////////////////////////////////////////////////////////////
/*

The code is seperated into two main parts:
In the lc(linear controller)_set_poses.cpp file is the publisher and the subscriber, aswell 
as a set of poses in an array.

The input, which is given by the dynamic reconfigure, will be used by the class robot.
The robot class contains the linear controller.

The controller is a Proportional Controller with the Paramters rho, alpha and beta.

The parameters have been empirical testet, and it appear that following values for the parameters had the best results:
rho: 0.01
alpha: 0.3
beta:0.1

Furthermore, if the robot moves away from the target, it should stop, but it continues spinning.


*//////////////////////////////////////////////////////////////////////

double x_G, y_G;
int theta_G=0*M_PI/180;
double x_R, y_R, theta_R;
double x_R_Gesch;
double k_rho=0.2;
double k_alpha=0.3;
double k_beta=0.3;
double kd=0.02;
double theta_R_Geschw;
bool on_off_switch;
double rho=0;
double alpha=0;
double beta=0;

double current_rho;
double last_rho;

int i=0;

double x_G1, y_G1, theta_G1, x_G2, y_G2, theta_G2, x_G3, y_G3, theta_G3, x_G4, y_G4, theta_G4;


void callback(eppacher_hw2::dynamic_reconfigure_param_2Config &config, uint32_t level) 
{
 ROS_INFO("Reconfigure Request: %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf  %d",
            x_G1=config.x_param_1,
            y_G1=config.y_param_1,
            theta_G1=(config.theta_goal_param_1)*M_PI/180,

            x_G2=config.x_param_2,
            y_G2=config.y_param_2,
            theta_G2=(config.theta_goal_param_2)*M_PI/180,

            x_G3=config.x_param_3,
            y_G3=config.y_param_3,
            theta_G3=(config.theta_goal_param_3)*M_PI/180,

            x_G4=config.x_param_4,
            y_G4=config.y_param_4,
            theta_G4=(config.theta_goal_param_4)*M_PI/180,


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
    dynamic_reconfigure::Server<eppacher_hw2::dynamic_reconfigure_param_2Config> server;
    ros::Rate loop_rate(1);
    geometry_msgs::Twist cmd_msg;
    dynamic_reconfigure::Server<eppacher_hw2::dynamic_reconfigure_param_2Config>::CallbackType f;
    f = boost::bind(&callback, _1, _2);
    server.setCallback(f);

    robot turtlebot;
    
    while (ros::ok())
    {
        double arrayPose[4][3];
        arrayPose[0][0]=x_G1;
        arrayPose[0][1]=y_G1;
        arrayPose[0][2]=theta_G1;

        arrayPose[1][0]=x_G2;
        arrayPose[1][1]=y_G2;
        arrayPose[1][2]=theta_G2;

        arrayPose[2][0]=x_G3;
        arrayPose[2][1]=y_G3;
        arrayPose[2][2]=theta_G3;

        arrayPose[3][0]=x_G4;
        arrayPose[3][1]=y_G4;
        arrayPose[3][2]=theta_G4;

        if(turtlebot.goal_reached==1)
        {
            turtlebot.goal_reached=0;
            i++;
            if(i==5)
            {
                //ros::shutdown();
            }
        }
        

        
        x_G=arrayPose[i][0];
        y_G=arrayPose[i][1];
        theta_G=arrayPose[i][2];

        turtlebot.linear_control( x_R,  y_R,  theta_R,  x_G,  y_G,  theta_G,  k_alpha,  k_beta, k_rho,  on_off_switch, current_rho, last_rho);
        ROS_INFO("                                                                         ");
        ROS_INFO("rho %lf alpha %lf beta %lf",turtlebot.rho,turtlebot.alpha,turtlebot.beta);
        ROS_INFO("v %lf w %lf",cmd_msg.linear.x,cmd_msg.angular.z);
        ROS_INFO("x_G %lf y_G %lf",turtlebot.x_G, turtlebot.y_G);
        ROS_INFO("x_R %lf y_R %lf",x_R, y_R);
        ROS_INFO("x_G %lf y_G %lf",x_G, y_G);
        ROS_INFO("                                                                         ");

     
        cmd_msg.linear.x=turtlebot.x_R_Gesch;
        cmd_msg.angular.z=turtlebot.theta_R_Geschw;
        linera_control_pub.publish(cmd_msg);
        last_rho=turtlebot.current_rho;


        ros::spinOnce();
        loop_rate.sleep();
    }

  return 0;
}











        /*
        if(on_off_switch==true)
        {
            rho=sqrt(pow((x_G-x_R),2)+pow((y_G-y_R),2));
            alpha=theta_R+atan2(((y_G-y_R)*(y_G-y_R)),((x_G-x_R)*(x_G-x_R)));
            beta=theta_G-theta_R-alpha;
            theta_R_Geschw=k_alpha*alpha+k_beta*beta;
            x_R_Gesch=k_rho*rho;
            current_rho=rho;
        }
        if(current_rho>last_rho)
        {
            x_R_Gesch=0;
        }
        if(rho<0.25)
        {
            x_R_Gesch=0;
        }

        if(on_off_switch==false)
        {
            x_R_Gesch=0;
            theta_R_Geschw=0;
        }
        */