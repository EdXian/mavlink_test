cmake_minimum_required(VERSION 2.8)

project(example)


add_executable(
imu imu.cpp

)

target_link_libraries(
imu mavlink_control
)






