cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
set(example true CACHE BOOL "Build Example")



project(mavlink_test)

include_directories(
    mavlink
    mavlink/include
    mavlink/include/mavlink
    mavlink/include/mavlink/v1.0
    mavlink/include/mavlink/v1.0/common
    mavlink_control
 )

add_library(serial_port 
		mavlink_control/serial_port.h 
		mavlink_control/serial_port.cpp)



add_library(mavlink_control
    mavlink_control/mavlink_control.h
     mavlink_control/mavlink_control.cpp
    )
target_link_libraries(mavlink_control      serial_port)

if(example)
add_subdirectory(example)

else()


endif()
add_executable(${PROJECT_NAME} "main.cpp"    )
target_link_libraries(
    ${PROJECT_NAME}
    mavlink_control


)




