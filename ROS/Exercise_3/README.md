# Ros Assignment 3

The new updated launch file includes
```
<?xml version="1.0" encoding="utf-8"?>

<launch>
  <include file="$(find smb_gazebo)/launch/smb_gazebo.launch">
    <arg name="world_file" value="$(find smb_highlevel_controller)/worlds/singlePillar.world"/>
    <arg name="laser_enabled" value="true"/>
  </include>
  <node name="pillar_controller" pkg="smb_highlevel_controller" type="smb_highlevel_controller_node" output="screen"/>
  <rosparam command="load" file="$(find smb_highlevel_controller)/config/params.yaml"/>
  <node pkg="rviz" type="rviz" name="rviz"/>
</launch>
```
The CMakeLists should include
```
cmake_minimum_required(VERSION 3.0.2)
project(smb_highlevel_controller)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
  sensor_msgs
  geometry_msgs  # Add geometry_msgs here
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES smb_highlevel_controller
  CATKIN_DEPENDS rospy std_msgs sensor_msgs geometry_msgs  # Add geometry_msgs here
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

# Add any source files for your node here
add_executable(smb_highlevel_controller_node src/SmbHighlevelController.cpp)

target_link_libraries(smb_highlevel_controller_node
  ${catkin_LIBRARIES}
)
```
And also updated the package.xml accordingly<br>

Initizialing the publisher
```
ros::NodeHandle nh;
ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
```

Call back subscription
```
ros::Subscriber laser_scan_sub = nh.subscribe("laser_scan_topic", 10, LaserScanCallback);
```

This is how I implemented a publisher in node for the /cmd_vel topic to send Twist commands.<br>
For the fifth instruuction , the params.yaml file will be updated accordingly
```
p_gain: 1.0 
target_distance: 1.0
```
The smb_gazebo.launch file would be updated
```
<launch>
  <rosparam file="$(find smb_highlevel_controller)/config/params.yaml" command="load" />
  <include file="$(find smb_gazebo)/launch/smb_gazebo.launch">
    <arg name="world_file" value="$(find smb_gazebo)/worlds/singlePillar.world"/>
    <arg name="laser_scan_min_height" value="-0.2"/>
    <arg name="laser_scan_max_height" value="1.0"/>
  </include>
  <node name="pillar_controller" pkg="smb_highlevel_controller" type="SmbHighlevelController" output="screen"/>
  <node pkg="rviz" type="rviz" name="rviz"/>
</launch>
```

And then the p controller logic can be implemented easily, the code would b
```
void SmbHighlevelController::LaserScanCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
    // Find the minimum distance to the pillar
    double min_distance = *std::min_element(scan->ranges.begin(), scan->ranges.end());

    // Calculate the error (difference between target and measured distance)
    double error = target_distance_ - min_distance;

    // Calculate control command using P controller
    double linear_speed = p_gain_ * error;
    double angular_speed = 0.0;  // Keep angular speed zero for now

    // Create and publish Twist message
    geometry_msgs::Twist cmd_msg;
    cmd_msg.linear.x = std::max(0.0, linear_speed);  // Ensure non-negative speed
    cmd_msg.angular.z = angular_speed;
    cmd_vel_pub_.publish(cmd_msg);
}
```
For the remaining steps I ran rviz then added the RobotModel plugin and TF display plugin.