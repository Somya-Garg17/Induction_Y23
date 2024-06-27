#include <smb_highlevel_controller/SmbHighlevelController.hpp>
#include <algorithm>

namespace smb_highlevel_controller {

SmbHighlevelController::SmbHighlevelController(ros::NodeHandle& nodeHandle) :
  nodeHandle_(nodeHandle)
{
  std::string scanTopic;
  int queueSize;

  nodeHandle_.param("scan_topic", scanTopic, std::string("/scan"));
  nodeHandle_.param("queue_size", queueSize, 10);

  scanSubscriber_ = nodeHandle_.subscribe(scanTopic, queueSize, &SmbHighlevelController::scanCallback, this);
}

SmbHighlevelController::~SmbHighlevelController()
{
}

void SmbHighlevelController::scanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  if (!msg->ranges.empty())
  {
    float minDistance = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    ROS_INFO("Smallest distance: %f", minDistance);
  }
  else
  {
    ROS_WARN("Received an empty range vector");
  }
}

} /* namespace */

