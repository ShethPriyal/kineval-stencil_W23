#include "Fetch_Controller.hpp"
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>
#include <vector>


Fetch_Controller::Fetch_Controller(ros::NodeHandle &nh)
{
    nh_ = nh;
   
    //TODO: initialize a publisher that is set to the channel "/cmd_vel"
    publisher_  = nh_.advertise<geometry_msgs::Twist>("/cmd_vel",1);
    //TODO: initialize a subscriber that is set to the channel "/base_scan". Set its callback function to be Laser_Scan_Callback
    subscriber_ = nh_.subscribe("/base_scan",50, &Fetch_Controller::Laser_Scan_Callback,this);
    
  
}

void Fetch_Controller::Laser_Scan_Callback(const sensor_msgs::LaserScan::ConstPtr &msg_laser_scan)
{
    /*TODO: 
    Given the incoming laser scan message, find the minimium distance of the front facing scans
    Hint: The laser scan measuring directly in front of the robot will be the scan at the middle of the array laser scans. 
    So for finding the minimum, we will ONLY consider the 120 laser scans in the middle of the array of laser scans. 
    If the minimum scan in this direction is greater than 1m, drive forward. 
    Otherwise, turn left. 
    */  
        std_msgs::Float32 min_distance;
        min_distance.data = msg_laser_scan->range_max;
        //std_msgs::Int8 StartPoint;
       // StartPoint.data = 270;//(msg_laser_scan->ranges.size()/2)-60;
        //std_msgs::Int8 EndPoint;
        //EndPoint.data = 390;//(msg_laser_scan->ranges.size()/2)+60;
        int EndPoint = (msg_laser_scan->ranges.size()/2) +60;
        int StartPoint = (msg_laser_scan->ranges.size()/2)-60;
        /**** if(EndPoint>msg_laser_scan->ranges.size())
        {
           // cout<<"Out of range index for laser scan"<<endl;
           /// continue;
        }***/
        //std_msgs::Int8 i;
        std_msgs::Float32 LidarData;
        
        ROS_INFO("%s", "EndPoint.data: ");
        ROS_INFO("%f", EndPoint);
        for (int i = StartPoint;i<EndPoint;i++)  
        {
            LidarData.data = msg_laser_scan->ranges[i] ;
            if( LidarData.data < min_distance.data && LidarData.data > 0 )
            {
                min_distance.data=LidarData.data;           
            }
        } 

        //Check condition to send out commands.
        geometry_msgs::Twist msg;
        std_msgs::Float32 Threshold;
        Threshold.data = 1;
        if(min_distance.data>Threshold.data)
        {
            ROS_INFO("%s", "Min Distance is: ");
            ROS_INFO("%f", min_distance.data);
            ROS_INFO("%s", "Moving straight");
            msg.linear.x = 0.5; //m/s
            //drive forward
        }
        else
        {
            ROS_INFO("%s", "turning left");
            msg.angular.z = 1.0;
        }
        publisher_.publish(msg);
        return;
}
