#include "ros/ros.h"


//using namespace std;


class robot
{
private:
//protected:
public:
    double x_G, y_G;
    int theta_G=0*M_PI/180;
    double x_R, y_R, theta_R;
    double x_R_Gesch;
    double k_rho;
    double k_alpha;
    double k_beta;
    double kd;
    double theta_R_Geschw;
    bool on_off_switch;
    double rho;
    double alpha;
    double beta;
    double current_rho=0;
    double last_rho=0;
    bool goal_reached=0;
    

    robot();

    void linear_control(double x_R, double y_R, double theta_R, double x_G, double y_G, double theta_G, double k_alpha, double k_beta,double k_rho, bool on_off_switch,double current_rho, double last_rho );
    /*
    double getRho();

    double getCurrent_Rho();

    double getLast_Rho();

    bool getGoal_reached();

    void setGoal_reached();

    void setCurrent_Rho(double rho);
    */


};



robot::robot(){}

void robot::linear_control(double x_R, double y_R, double theta_R, double x_G, double y_G, double theta_G, double k_alpha, double k_beta,double k_rho, bool on_off_switch, double current_rho, double last_rho )
{
    
        this->x_R=x_R;
        this->y_R=y_R;
        this->theta_R=theta_R;

        this->x_G=x_G;
        this->y_G=y_G;
        this->theta_G=theta_G;

        this->k_alpha=k_alpha;
        this->k_beta=k_beta;
        this->k_rho=k_rho;

        this->current_rho=current_rho;
        this->last_rho=last_rho;

        this->on_off_switch=on_off_switch;

        goal_reached=0;

      if(this->on_off_switch==true)
        {
            this->rho=sqrt(pow((this->x_G-this->x_R),2)+pow((this->y_G-this->y_R),2));
            this->alpha=this->theta_R+atan2(pow((this->y_G-this->y_R),2),pow((this->x_G-this->x_R),2));
            this->beta=this->theta_G-theta_R-this->alpha;
            this->theta_R_Geschw=this->k_alpha*this->alpha+this->k_beta*this->beta;
            this->x_R_Gesch=this->k_rho*this->rho;
            this->current_rho=this->rho;
        }
        
        if(this->current_rho>last_rho)
        {
            x_R_Gesch=0;
        }
        
        if(rho<0.5)
        {
            x_R_Gesch=0;
            goal_reached=1;
        }

        if(on_off_switch==false)
        {
            x_R_Gesch=0;
            theta_R_Geschw=0;
        }
        last_rho=current_rho;
        
}

/*
double robot::getRho()
{
    return this->rho;
}

double robot::getCurrent_Rho()
{
    return this->rho;
}

double robot::getLast_Rho()
{
    return this->rho;
}

bool robot::getGoal_reached()
{
    return this->goal_reached;
}

void robot::setGoal_reached(bool goal_reached)
{
    this->goal_reached=goal_reached;
}

void robot::setCurrent_Rho(double rho1)
{
    this->current_rho=rho1;
}

*/