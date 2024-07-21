cmake_minimum_required(VERSION 3.28.0)
project(Ball_Balacing)

find_package(OpenCV REQUIRED) 
  
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "     mingw version:${CMAKE_CXX_COMPILER_VERSION}")

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(ballBalacing main.cpp controller.cpp usb.cpp)

target_link_libraries(ballBalacing ${OpenCV_LIBS}) 

