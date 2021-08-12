cmake_minimum_required(VERSION 3.1) 
project(yolo-trt VERSION 1.0)            
enable_language(CUDA)

set(CMAKE_CXX_COMPILIER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -Wno-write-strings ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")


enable_language(CUDA)
find_package(CUDA REQUIRED)
get_filename_component(CUDA_LIBRARY_DIR ${CUDA_CUDART_LIBRARY} DIRECTORY)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-L${CUDA_LIBRARY_DIR}")
set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")



find_package(OpenCV REQUIRED)
find_package (Eigen3  REQUIRED NO_MODULE)
find_package(TensorflowCC REQUIRED)
find_package(Boost COMPONENTS program_options filesystem REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources  
src/mot/**
include/mot_config_files.hpp
)

add_library(detector SHARED ${sources})

target_include_directories(detector PRIVATE 
${OpenCV_INCLUDE_DIRS} 
${CUDA_TOOLKIT_ROOT_DIR}/include  
${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(detector TensorflowCC::TensorflowCC Eigen3::Eigen nvinfer nvinfer_plugin nvparsers "stdc++fs" ${OpenCV_LIBS} ${Boost_LIBRARIES})


######END SORT ############

file(GLOB_RECURSE sources3  ${CMAKE_CURRENT_SOURCE_DIR}/include/objectTracker.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/objectTracker.cpp
)

add_executable(ros2Mot  main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/objectTracker.cpp)	
target_include_directories(ros2Mot PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/src/ 
${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(ros2Mot detector)


