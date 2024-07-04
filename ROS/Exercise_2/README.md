# ROS Assignment 2

I downloaded the smb_highlevel_controller.zip from the course website, then inspected the CMakeLists.txt and package.xml.<br>
They can be seen in the ros_ws/src/smb_highlevel_controller directory.<br>
Now we have to create a subscriber to the /scan topic.<br>
In order to do this , in src/SmbHighlevelController.cpp, I created a subscriber to the /scan topic and updated the SmbHighlevelController.hpp accordingly.<br>
Then I added the parameter file params.yaml
```
scan_topic: "/scan"
queue_size: 10
```
And modified the launch file to pass the argument, rviz added and odom set as fixed frame. 
```
<launch>
  <arg name="laser_enabled" default="true"/>
  <param name="laser_enabled" value="$(arg laser_enabled)"/>
  <rosparam file="$(find smb_highlevel_controller)/config/params.yaml" command="load"/>
  <node pkg="smb_highlevel_controller" type="smb_highlevel_controller" name="smb_highlevel_controller" output="screen"/>
  <node pkg="rviz" type="rviz" name="rviz" args="-d $(find smb_highlevel_controller)/config/default.rviz" />
</launch>
```

