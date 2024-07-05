cmake_minimum_required(VERSION 3.0.2)
project(smb_description)

find_package(catkin REQUIRED COMPONENTS
  urdf
  xacro
  rviz
)

catkin_package()

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(DIRECTORY rviz
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/rviz
)

