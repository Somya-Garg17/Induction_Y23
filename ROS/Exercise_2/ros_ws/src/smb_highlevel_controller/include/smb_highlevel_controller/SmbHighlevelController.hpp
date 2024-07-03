#pragma once

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

namespace smb_highlevel_controller {

class SmbHighlevelController {
public:
    SmbHighlevelController(ros::NodeHandle& nodeHandle);
    virtual ~SmbHighlevelController();

private:
    void scanCallback(const sensor_msgs::LaserScan::ConstPtr& msg);

    ros::NodeHandle nodeHandle_;
    ros::Subscriber scanSubscriber_;
};

} /* namespace */

