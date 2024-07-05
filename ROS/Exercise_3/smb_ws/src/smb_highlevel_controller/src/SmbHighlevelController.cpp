#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>

class SmbHighlevelController {
public:
    SmbHighlevelController() {
        // Initialize ROS node handle and subscribers/publishers
        ros::NodeHandle nh;
        laser_sub_ = nh.subscribe("/scan", 1, &SmbHighlevelController::laserCallback, this);
        cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

        // Get parameters from the parameter server
        nh.param("k_p", k_p_, 1.0);
        nh.param("max_speed", max_speed_, 0.5);
        nh.param("min_distance", min_distance_, 0.5);
    }

private:
    ros::Subscriber laser_sub_;
    ros::Publisher cmd_vel_pub_;
    double k_p_;
    double max_speed_;
    double min_distance_;

    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {
        // Find the distance to the nearest pillar
        float closest_distance = std::numeric_limits<float>::infinity();
        int closest_index = -1;

        for (int i = 0; i < msg->ranges.size(); ++i) {
            if (msg->ranges[i] < closest_distance) {
                closest_distance = msg->ranges[i];
                closest_index = i;
            }
        }

        // Calculate control command
        geometry_msgs::Twist cmd_vel;
        if (closest_distance < min_distance_) {
            // The pillar is too close
            cmd_vel.linear.x = -max_speed_;
        } else {
            // The pillar is too far away
            cmd_vel.linear.x = k_p_ * (closest_distance - min_distance_);
            if (cmd_vel.linear.x > max_speed_) {
                cmd_vel.linear.x = max_speed_;
            }
        }

        // Publish the velocity command
        cmd_vel_pub_.publish(cmd_vel);
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "smb_highlevel_controller");
    SmbHighlevelController controller;
    ros::spin();
    return 0;
}

