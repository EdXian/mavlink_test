cmake_minimum_required(VERSION 3.1.0)

project(vrpn_test)
find_package(Threads REQUIRED)

include_directories(
/usr/local/include

#/usr/local/bin

)




add_executable(${PROJECT_NAME} "main.cpp" )
target_link_libraries(${PROJECT_NAME}  Threads::Threads vrpn quat)
