# -- lens_calib --
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
project (lens_calib CXX)
find_package(OpenCV REQUIRED)

set (get_sharpness_SRCS src/get_sharpness.cpp)
set (locate_target_SRCS src/locate_target.cpp)

add_executable(get_sharpness ${get_sharpness_SRCS})
add_executable(locate_target ${locate_target_SRCS})

target_link_libraries(get_sharpness ${OpenCV_LIBS})
target_link_libraries(locate_target ${OpenCV_LIBS})
