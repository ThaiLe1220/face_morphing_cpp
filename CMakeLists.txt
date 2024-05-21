cmake_minimum_required(VERSION 3.10)
project(face_morph)

# C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Enable compiler optimizations for Release build
set(CMAKE_BUILD_TYPE "Release")

# Add options for profile generation and usage
option(USE_PROFILE_GENERATE "Enable profile generation" OFF)
option(USE_PROFILE_USE "Enable profile use" OFF)

# Locate necessary packages
find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)

# Optimization flags with profile-guided optimization options
if(USE_PROFILE_GENERATE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -march=native -flto -fomit-frame-pointer -funroll-loops -fprofile-generate")
elseif(USE_PROFILE_USE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -march=native -flto -fomit-frame-pointer -funroll-loops -fprofile-use")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -march=native -flto -fomit-frame-pointer -funroll-loops")
endif()

# Include directories for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)

# Add executables
# add_executable(main main.cc
#     utils/face_correspondences.cc
#     utils/delaunay_triangulation.cc
#     utils/face_morph.cc
#     utils/video_handler.cc)

add_executable(test test.cc
    utils/image_processor.cc
    utils/video_processor.cc)

# add_executable(frame_extract frame_extract.cc)

# Link libraries to the executable
# target_link_libraries(main ${OpenCV_LIBS} dlib::dlib)
target_link_libraries(test ${OpenCV_LIBS})
# target_link_libraries(frame_extract ${OpenCV_LIBS})
