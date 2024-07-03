#include <smb_highlevel_controller/SmbHighlevelController.hpp>
#include <limits>
#include <algorithm>

namespace smb_highlevel_controller {

SmbHighlevelController::SmbHighlevelController(ros::NodeHandle& nodeHandle) :
  nodeHandle_(nodeHandle)
{
    // Initialize the subscriber
    std::string scanTopic;
    int queueSize;
    nodeHandle_.getParam("scan_topic", scanTopic);
    nodeHandle_.getParam("queue_size", queueSize);

    scanSubscriber_ = nodeHandle_.subscribe(scanTopic, queueSize, &SmbHighlevelController::scanCallback, this);
}

SmbHighlevelController::~SmbHighlevelController()
{
}

void SmbHighlevelController::scanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    // Find the smallest distance
    auto minElement = std::min_element(msg->ranges.begin(), msg->ranges.end());

    if (minElement != msg->ranges.end()) {
        ROS_INFO("Smallest distance: %f", *minElement);
    } else {
        ROS_WARN("No valid range data received.");
    }
}

} /* namespace */

