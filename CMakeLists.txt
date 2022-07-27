cmake_minimum_required(VERSION 3.22)
project(video)

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR C:/Users/Philip/Downloads/opencv/build)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(video main.cpp Octree.hpp Color.hpp Coord.hpp Media.hpp Compressor.hpp)
target_link_libraries( video ${OpenCV_LIBS} )
