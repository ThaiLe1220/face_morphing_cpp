cmake_minimum_required(VERSION 3.10)
project(face_morph)

# C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Locate necessary packages
find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)

# Include directories for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)

# Add executables
add_executable(main main.cc
    utils/face_correspondences.cc
    utils/delaunay_triangulation.cc
    utils/face_morph.cc)

add_executable(frame_extract frame_extract.cc)

# Link libraries to the executable
target_link_libraries(main ${OpenCV_LIBS} dlib::dlib)
target_link_libraries(frame_extract ${OpenCV_LIBS} dlib::dlib)
