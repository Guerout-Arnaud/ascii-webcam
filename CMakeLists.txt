cmake_minimum_required(VERSION 3.1)
project( ascii-webcam )
set (CMAKE_CXX_STANDARD 17)



find_package( OpenCV REQUIRED )
# find_package( yuv REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set (COMMON_SRC
    src/main.cpp
    src/common/ArgParser.cpp
    src/common/AsciiProcessor.cpp
)

set (LINUX_SRC
    src/linux/VideoCapture.cpp
    src/linux/VideoOutput.cpp
)

add_executable( ${PROJECT_NAME} ${COMMON_SRC} ${LINUX_SRC})
target_include_directories( ${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS} PUBLIC ./include)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS})

# add_executable( capture .local/bkup.cpp)
# target_include_directories( capture PUBLIC ${OpenCV_INCLUDE_DIRS})
# target_compile_definitions( capture PUBLIC "-std=c++17" )
# target_link_libraries( capture ${OpenCV_LIBS} yuv)

